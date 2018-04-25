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
    if (shade.depth == 1)
        return vec3();

    return Emissive::shade(shade);
}
