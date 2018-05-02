#include "photonmap.h"
#include "debug.h"
#include "core/world.h"
#include "bsdf/lambertian.h"

int PhotonMap::MIN_PHOTONS_REQUIRED = 8;
float PhotonMap::SAMPLE_DISTANCE = 1e-2;

Photon::Photon(dvec3 position, dvec3 direction, vec3 power, short bounce) :
    position(position), power(power), bounce(bounce)
{
    direction = normalize(direction);
    azim = atan2(direction.x, direction.z);
    polar = acos(direction.y);
}

dvec3 Photon::getDirection() {
    double x = sin(polar) * sin(azim);
    double y = cos(polar);
    double z = sin(polar) * cos(azim);
    return dvec3(x, y, z);
}

void Photon::output() {
    cout << "address: " << this << endl;
    cout << "position: ";
    Debug::log(position);
    cout << "direction: ";
    dvec3 dir = getDirection();
    Debug::log(dir);
    cout << "power: ";
    Debug::log(power);
}

PhotonMap::PhotonMap(World &world) : world(world) {
    root = nullptr;
    photonVec.reserve(world.lights.size() * World::NUM_PHOTONS_PER_LIGHT);
}

void PhotonMap::build() {
    this->root = buildTree(photonVec);
    photonVec.clear();
}

void PhotonMap::addPhoton(dvec3 position, dvec3 direction, vec3 power) {
    photonVec.push_back(new Photon(position, direction, power));
}

void PhotonMap::scalePhotonPower(float scale) {
    for (int i = lastIndex; i < photonVec.size(); i++) 
        photonVec[i]->power *= vec3(scale);
    lastIndex = photonVec.size();
}

BoundingBox PhotonMap::calcBoundingBox(vector<Photon *> &photons) {
    dvec3 minCoord(numeric_limits<double>::max());
    dvec3 maxCoord(numeric_limits<double>::min());
    for (auto photon : photons) {
        minCoord = min(minCoord, photon->position);
        maxCoord = max(maxCoord, photon->position);
    }
    return BoundingBox(minCoord - dvec3(EPSILON), maxCoord + dvec3(EPSILON));
}

PhotonMap::TreeNode * PhotonMap::buildTree(vector<Photon *> &photons) {
    auto node = new TreeNode();
    if (photons.size() == 0)
        return nullptr;
    else if (photons.size() == 1) {
        node->data = photons[0];
        return node;
    }

    BoundingBox bounding = calcBoundingBox(photons);
    int dim = selectDimension(bounding);
    vector<Photon *> below, above;
    auto median = divide(photons, dim, below, above);

    node->dim = dim;
    node->data = median;
    node->left = buildTree(below);
    node->right = buildTree(above);
    return node;
}

int PhotonMap::selectDimension(BoundingBox bounding_box) {
    dvec3 range = bounding_box.vertMax - bounding_box.vertMin;
    int dim;
    Math::maxComponent(range, dim);
    return dim;
}

Photon * PhotonMap::divide(vector<Photon *> &photons, int dim, vector<Photon *> &below, 
    vector<Photon *> &above)
{
    quickSort(photons, dim, 0, photons.size() - 1);
    vector<Photon *>::iterator medianIte = photons.begin() + (photons.size() - 1) / 2;

    if (photons.begin() <= medianIte)
        below.insert(below.begin(), photons.begin(), medianIte);
    if (medianIte + 1 <= photons.end())
        above.insert(above.begin(), medianIte + 1, photons.end());

    return *medianIte;
}

void PhotonMap::quickSort(vector<Photon *> &photons, int dim, int low, int high) {
    if (low >= high) return;
    int partition = getPartition(photons, dim, low, high);
    quickSort(photons, dim, low, partition - 1);
    quickSort(photons, dim, partition + 1, high);
}

int PhotonMap::getPartition(vector<Photon *> &photons, int dim, int low, int high) {
    double pivot = photons[low]->position[dim];
    while (low < high) {
        while (low < high && pivot <= photons[high]->position[dim]) high--;
        swap(photons[low], photons[high]);
        while (low < high && pivot >= photons[low]->position[dim]) low++;
        swap(photons[low], photons[high]);
    }
    return low;
}

void PhotonMap::locatePhotons(NearestPhotons *np, PhotonMap::TreeNode *node) {
    if (node == nullptr) return;

    double splitDist = np->position[node->dim] - node->data->position[node->dim]; // signed distance to splitting plane
    double splitDistSq = splitDist * splitDist;
    if (splitDist < 0)  { // on the left side of node
        locatePhotons(np, node->left); // search left subtree
        if (splitDistSq < np->maxDistSq) // can search right subtree
            locatePhotons(np, node->right);
    } else {
        locatePhotons(np, node->right);
        if (splitDistSq < np->maxDistSq)
            locatePhotons(np, node->left);
    }

    double dist = Math::distSq(node->data->position, np->position); // real squared distance to photon
    double distSq = dist * dist;
    if (distSq < np->maxDistSq) {
        np->photons.push(node->data);
        if (np->photons.size() > MIN_PHOTONS_REQUIRED) {
            np->photons.pop();
            np->maxDistSq = Math::distSq(np->position, np->photons.top()->position);
        }
    }

}

vec3 PhotonMap::estimateIrradiance(Shade &shade, Lambertian *brdf)
{
    auto np = new NearestPhotons();
    np->maxDistSq = SAMPLE_DISTANCE * SAMPLE_DISTANCE;
    np->position = shade.hitPoint;
    function<bool(Photon *, Photon *)> compare = 
        [&] (Photon *a, Photon *b) -> 
        bool { return Math::distSq(a->position, np->position) > Math::distSq(b->position, np->position); };
    np->photons = priority_queue<Photon *, vector<Photon *>, decltype(compare)> (compare);
    locatePhotons(np, root);

    if (np->photons.size() < MIN_PHOTONS_REQUIRED) return Color::BLACK;
    vec3 irradiance;
    while (np->photons.size() > 0) {
        auto photon = np->photons.top();
        dvec3 dir = -photon->getDirection();
        dvec3 in;
        float nDotIn = dot(shade.normal, dir);
        if (nDotIn > 0.0) 
            irradiance += brdf->calcBRDF(shade, in, dir) * photon->power * nDotIn;
        np->photons.pop();
    }
    delete np;

    irradiance /= (PI * np->maxDistSq);
    return irradiance;
}

void PhotonMap::clear(PhotonMap::TreeNode *node) {
    if (node->data) delete node->data;
    if (node->left) clear(node->left);
    if (node->right) clear(node->right);
    delete node;
}

PhotonMap::~PhotonMap() {
    clear(root);
}

void PhotonMap::output() {
    output(root, 0);
}

void PhotonMap::output(PhotonMap::TreeNode *node, int depth) {
    cout << "depth: " << depth << " dim: " << node->dim << endl;
    node->data->output();
    if (node->left) output(node->left, depth + 1);
    if (node->right) output(node->right, depth + 1);
}
