#pragma once

#include "core/light.h"

class Ambient : public Light {
public:
    Ambient(vec3 color, float scale) : scale(scale), color(color) {}
    virtual dvec3 getDirection(Shade &shade) {
        return dvec3(0.0);
    }
    virtual vec3 incidRadiosity(Shade &shade) {
        return scale * color;
    }

private:
    float scale;
    vec3 color;
};