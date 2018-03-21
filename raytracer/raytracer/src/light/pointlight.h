#pragma once

#include "core/light.h"

class PointLight : public Light {
public:
    PointLight(dvec3 location, vec3 color, float scale) : 
        scale(scale), color(color), location(location) {}

    virtual bool inShadow(Ray &shadow_ray, Shade &shade) {
        double t;
        double d = distance(shadow_ray.origin, location);

        for (int i = 0; i < shade.world.lights.size(); i++) {
            Geometry *object = shade.world.objects[i];
            if (object->shadowIntersect(shadow_ray, t) && object->castShadow() && (t < d))
                return true;
        }
        
        return false;
    }

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