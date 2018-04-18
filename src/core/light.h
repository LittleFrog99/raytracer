#pragma once

#include "utilities.h"
#include "core/shade.h"

class Light {
public:
    virtual dvec3 calcDirection(Shade &shade) = 0;
    virtual vec3 incidRadiance(Shade &shade) { return vec3(); }
    virtual bool inShadow(Ray &shadow_ray, Shade &shade) = 0;
    virtual float probDenFunc(Shade &shade) { return 1.0; }
    virtual float geoTerm(Shade &shade) { return 1.0; }

    inline void toggleShadowCast(bool cast) {
        _castShadow = cast;
    }
    inline bool castShadow() {
        return _castShadow;
    }
    virtual ~Light() {}

protected:
    bool _castShadow = true;
};