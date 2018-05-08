#pragma once

#include "utilities.h"
#include "photon.h"
#include "geometry/bound/boundingbox.h"
#include <queue>

struct NearestPhotons {
    dvec3 position;
    int max = 0, found = 0;

    short gotHeap = false;
    double *distSq = nullptr;
    Photon **photons = nullptr;

    ~NearestPhotons();
};

class World;
class Lambertian;
struct Shade;

class PhotonMap : public BoxBounded {
public:
    static int CAUSTICS_SAMPLE_PHOTONS;
    static double CAUSTICS_SAMPLE_DISTANCE;
    static const int MIN_PHOTONS_REQUIRED;
    World *world;

    PhotonMap(World *world);
    void storeGlobal(Photon *photon);
    void storeCaustic(Photon *photon);
    void balance();
    vec3 estimateIrradiance(Shade &shade);
    void scalePhotonPower(float scale);
    ~PhotonMap();

private:
    int globMax, globStored = 0, globLast = 1;
    int causMax, causStored = 0, causLast = 1;
    Photon *global;
    Photon *caustics;

    void balanceSegment(Photon *map, Photon *pOrg, int index, int start, int end);
    void medianSplit(Photon *pOrg, int start, int end, int median, int dim);
    void locatePhotons(Photon *map, NearestPhotons *np, int index); // called by index = 1
};


/* struct NearestPhotons {
    dvec3 position;
    double maxDistSq;
    priority_queue<Photon *, vector<Photon *>, function<bool(Photon *, Photon *)>> photons;
};

class World;
class Lambertian;
struct Shade;

class PhotonMap {
public:
    static int MIN_PHOTONS_REQUIRED;
    static float SAMPLE_DISTANCE;

    PhotonMap(World &world);
    void addPhoton(dvec3 position, dvec3 direction, vec3 power);
    void scalePhotonPower(float scale);
    void build(); // call this method before use
    vec3 estimateIrradiance(Shade &shade, Lambertian *brdf);
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

    // Version 1: Tree node
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
*/
