#pragma once

#include "photonmap.h"
#include "core/world.h"

class PhotonTracer {
public:
    static void setWorld(World *world_ptr);
    static void tracePhoton(PhotonMap *map, Photon *photon);
    static PhotonScatter scatterBehavior(map<pair<float, float>, PhotonScatter> behavior); // using Russian Roulette
    
private:
    static World *worldP;
};