#pragma once

#include "core/light.h"

class Ambient : public Light {
public:
    Ambient(vec3 color, float intensity) : intensity(intensity), color(color) {
        _castShadow = false;
    }
    virtual bool inShadow(Ray &shadow_ray, Shade &shade)  {
        return false;
    }
    virtual dvec3 calcDirection(Shade &shade) {
        return dvec3(0.0);
    }
    virtual vec3 incidRadiance(Shade &shade) {
        return intensity * color;
    }

private:
    float intensity;
    vec3 color;
};