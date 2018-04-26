#pragma once

#include "utilities.h"
#include "photon.h"
#include "geometry/bound/boundingbox.h"
#include <queue>

struct NearestPhotons {
    dvec3 position;
    double maxDistSq;
    priority_queue<Photon *, vector<Photon *>, function<bool(Photon *, Photon *)>> photons;
};

class World;

class PhotonMap : public BoxBounded {
public:
    static constexpr int MIN_PHOTONS_REQUIRED = 8;
    static constexpr float DISTANCE_SCALE_FACTOR = 1e6;

    PhotonMap(World &world);
    void addPhoton(Photon *photon) { photonVec.push_back(photon); }
    void addPhoton(dvec3 position, dvec3 direction, vec3 power);
    void scalePhotonPower(float scale);
    void build(); // call this method before use
    vec3 estimateIrradiance(dvec3 position, dvec3 normal);
    void output();
    ~PhotonMap();

    World &world;

private:
    struct TreeNode {
        Photon *data = nullptr;
        int dim = 0;
        TreeNode *left = nullptr, *right = nullptr;
    };

    vector<Photon *> photonVec;
    int lastIndex = 0;
    TreeNode *root;

    TreeNode * buildTree(vector<Photon *> &photons);
    BoundingBox calcBoundingBox(vector<Photon *> &photons);
    int selectDimension(BoundingBox bounding_box);
    Photon * divide(vector<Photon *> &photons, int dim, vector<Photon *> &below, vector<Photon *> &above);
    void locatePhotons(NearestPhotons *np, TreeNode *node);

    void quickSort(vector<Photon *> &photons, int dim, int low, int high);
    int getPartition(vector<Photon *> &photons, int dim, int low, int high);

    void clear(TreeNode *node);
    void output(TreeNode *node, int depth);
};

