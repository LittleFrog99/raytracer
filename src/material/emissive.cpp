#include "emissive.h"
#include "core/shade.h"

vec3 Emissive::areaLightShade(Shade &shade) {
    return (-dot(shade.normal, shade.ray.direction) > 0.0) ?
        intensity * color : vec3();
}

vec3 Emissive::getEmissiveLight(Shade &shade) {
    return intensity * color;
}