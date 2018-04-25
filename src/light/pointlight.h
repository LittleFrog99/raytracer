#pragma once

#include "core/light.h"

class PointLight : public Light {
public:
    PointLight(dvec3 location, vec3 color, float intensity) : 
        intensity(intensity), color(color), location(location) {}

    virtual bool inShadow(Ray &shadow_ray, Shade &shade);

    virtual dvec3 calcDirection(Shade &shade) {
        return normalize(location - shade.hitPoint);
    }

    virtual vec3 incidRadiance(Shade &shade) {
        return intensity * color;
    }

private:
    float intensity;
    vec3 color;
    dvec3 location;
};