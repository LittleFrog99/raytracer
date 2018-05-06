#include "photontracer.h"
#include "debug.h"

World * PhotonTracer::worldP = nullptr;

void PhotonTracer::setWorld(World *world_ptr) {
    worldP = world_ptr;
}

void PhotonTracer::tracePhoton(PhotonMap *map, Photon *photon)
{
    if (photon == nullptr) return;
    if (photon->bounce > worldP->vp.maxDepth) return;
    
    Ray ray = Ray(photon->position, photon->getDirection());
    auto shade = worldP->intersectObjects(ray);
    if (shade.hasHit) {
        photon->position = shade.hitPoint;
        shade.materialP->photonInteract(shade, map, photon);
    }
}

PhotonScatter PhotonTracer::scatterBehavior(map<pair<float, float>, PhotonScatter> behavior)
{
    float value = roulette();
    for (auto pair : behavior) {
        if (value >= pair.first.first && value < pair.first.second)
            return pair.second;
    }
    return ABSORPTION;
}
