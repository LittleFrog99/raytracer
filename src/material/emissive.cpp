#include "emissive.h"
#include "core/shade.h"

vec3 Emissive::shade(Shade &shade) {
    return (-dot(shade.normal, shade.ray.direction) > 0.0) ?
        intensity * color : vec3();
}

vec3 Emissive::getEmissiveLight(Shade &shade) {
    return intensity * color;
}

vec3 Emissive::pathShade(Shade &shade) {
    return Emissive::shade(shade);
}

vec3 Emissive::photonShade(Shade &shade) {
    return getEmissiveLight(shade);
}

void Emissive::photonInteract(Shade &shade, PhotonMap *map, Photon *photon) {
    // nothing to do
}