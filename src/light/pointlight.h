#pragma once

#include "core/light.h"

class PointLight : public Light {
public:
    PointLight(dvec3 location, vec3 color, float scale) : 
        scale(scale), color(color), location(location) {}
    virtual dvec3 getDirection(Shade &shade) {
        return normalize(location - shade.hitPoint);
    }
    virtual vec3 incidRadiosity(Shade &shade) {
        return scale * color;
    }

private:
    float scale;
    vec3 color;
    dvec3 location;
};