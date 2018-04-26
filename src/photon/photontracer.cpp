#include "photontracer.h"

World * PhotonTracer::worldP = nullptr;

void PhotonTracer::setWorld(World *world_ptr) {
    worldP = world_ptr;
}

void PhotonTracer::tracePhoton(PhotonMap *map, Photon *photon) {
    Ray ray = Ray(photon->position, photon->getDirection());
    auto shade = worldP->intersectObjects(ray);
    if (shade.hasHit) {
        photon->position = shade.hitPoint;
        shade.materialP->photonInteract(shade, map, photon);
    }
    else
        delete photon;
}
