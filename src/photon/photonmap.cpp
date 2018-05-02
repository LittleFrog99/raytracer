#include "photonmap.h"
#include "debug.h"
#include "core/world.h"
#include "bsdf/lambertian.h"

dvec3 Photon::getDirection() {
    double x = sin(polar) * sin(azim);
    double y = cos(polar);
    double z = sin(polar) * cos(azim);
    return dvec3(x, y, z);
}

void Photon::setDirection(glm::dvec3 dir) {
    dir = normalize(dir);
    azim = atan2(dir.x, dir.z);
    polar = acos(dir.y);
}

NearestPhotons::~NearestPhotons() {
    delete[] distSq;
    delete[] photons;
}

PhotonMap::PhotonMap(World *world) : world(world)
{
    max = world->NUM_PHOTONS_PER_LIGHT * world->lights.size() * world->vp.maxDepth;
    photons = new Photon[max];
}

PhotonMap::~PhotonMap() {
    delete[] photons;
}

void PhotonMap::scalePhotonPower(float scale) {
    for (int i = last; i <= stored; i++)
        photons[i].power *= scale;
    last = stored + 1;
}

void PhotonMap::store(Photon *photon) {
    if (stored > max) return;
    photons[++stored] = Photon(*photon);
    bndBox.vertMax = glm::max(bndBox.vertMax, photon->position);
    bndBox.vertMin = glm::min(bndBox.vertMin, photon->position);
}

void PhotonMap::balance() {
    Photon *pOrg = new Photon[stored + 1];
    for (int i = 1; i <= stored; i++)
        pOrg[i] = photons[i];
    balanceSegment(pOrg, 1, 1, stored);
    delete[] pOrg;
}

void PhotonMap::balanceSegment(Photon *pOrg, int index, int start, int end)
{
    if (start == end) {
        photons[index] = pOrg[start];
        return;
    }

    int median = 1;
    while (4 * median <= end - start + 1) median <<= 1;
    if (3 * median <= end - start + 1)
        median = median * 2 + start - 1;
    else 
        median = end - median + 1;

    int dim;
    Math::maxComponent(bndBox.vertMax - bndBox.vertMin, dim);
    medianSplit(pOrg, start, end, median, dim);
    photons[index] = pOrg[median];
    photons[index].dim = dim;

    if (start < median) {
        double temp = bndBox.vertMax[dim];
        bndBox.vertMax[dim] = photons[index].position[dim];
        balanceSegment(pOrg, index * 2, start, median - 1);
        bndBox.vertMax[dim] = temp;
    }
    if (median < end) {
        double temp = bndBox.vertMin[dim];
        bndBox.vertMin[dim] = photons[index].position[dim];
        balanceSegment(pOrg, index * 2 + 1, median + 1, end);
        bndBox.vertMin[dim] = temp;
    }
}

void PhotonMap::medianSplit(Photon *pOrg, int start, int end, int median, int dim)
{
    int left = start, right = end;

    while (left < right) {
        double key = pOrg[right].position[dim];
        int i = left - 1, j = right;
        while (true) {
            while (pOrg[++i].position[dim] < key);
            while (pOrg[--j].position[dim] > key && j > left);
            if (i >= j) break;
            swap(pOrg[i], pOrg[j]);
        }

        swap(pOrg[i], pOrg[right]);
        if (i >= median) right = i - 1;
        if (i <= median) left = i + 1;
    }
}

void PhotonMap::locatePhotons(NearestPhotons *np, int index)
{
    if (index > stored) return;
    Photon *photon = &photons[index];

    if (index * 2 <= stored) {
        double dist = np->position[photon->dim] - photon->position[photon->dim];
        if (dist < 0) {
            locatePhotons(np, index * 2);
            if (dist * dist < np->distSq[0])
                locatePhotons(np, index * 2 + 1);
        } else {
            locatePhotons(np, index * 2 + 1);
            if (dist * dist < np->distSq[0])
                locatePhotons(np, index * 2);
        }
    }

    double distSq = Math::distSq(photon->position, np->position);
    if (distSq > np->distSq[0]) return;

    if (np->found < np->max) {
        np->found++;
        np->distSq[np->found] = distSq;
        np->photons[np->found] = photon;
    } else {
        if (!np->gotHeap) {
            for (int i = np->found >> 1; i >= 1; i--) {
                int parent = i;
                Photon *tmpPhoton = np->photons[i];
                double tmpDistSq = np->distSq[i];
                while ((parent << 1) <= np->found) {
                    int j = parent << 1;
                    if (j + 1 <= np->found && np->distSq[j] < np->distSq[j + 1]) j++;
                    if (tmpDistSq >= np->distSq[j]) break;

                    np->photons[parent] = np->photons[j];
                    np->distSq[parent] = np->distSq[j];
                    parent = j;
                }
                np->photons[parent] = tmpPhoton;
                np->distSq[parent] = tmpDistSq;
            }
            np->gotHeap = true;
        }

        int parent = 1;
        while ((parent << 1) <= np->found) {
            int j = parent << 1;
            if (j + 1 <= np->found && np->distSq[j] < np->distSq[j + 1]) j++;
            if (distSq > np->distSq[j]) break;

            np->photons[parent] = np->photons[j];
            np->distSq[parent] = np->distSq[j];
            parent = j;
        }
        np->photons[parent] = photon;
        np->distSq[parent] = distSq;
        np->distSq[0] = np->distSq[1];
    }
}

vec3 PhotonMap::estimateIrradiance(Shade &shade)
{
    vec3 color;
    double max_dist = world->vp.sampleDist;
    int num = world->vp.samplePhotons;

    NearestPhotons np;
    np.position = shade.hitPoint;
    np.max = num;
    np.distSq = new double[num + 1];
    np.distSq[0] = max_dist * max_dist;
    np.photons = new Photon *[num + 1];

    locatePhotons(&np, 1);
    if (np.found <= 8) return color;

    for (int i = 1; i <= np.found; i++) {
        dvec3 dir = -np.photons[i]->getDirection();
        if (dot(shade.normal, dir) > 0) 
            color += np.photons[i]->power;
    }

    return color / float(PI * np.distSq[0]);
}


// The following is a much slower implementation using standard library

/* int PhotonMap::MIN_PHOTONS_REQUIRED = 8;
float PhotonMap::SAMPLE_DISTANCE = 1e-2;

Photon::Photon(dvec3 position, dvec3 direction, vec3 power, short bounce) :
    position(position), power(power), bounce(bounce)
{
    direction = normalize(direction);
    azim = atan2(direction.x, direction.z);
    polar = acos(direction.y);
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
*/