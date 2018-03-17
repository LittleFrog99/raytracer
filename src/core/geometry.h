#pragma once

#include "core/shade.h"
#include "core/ray.h"
#include "core/material.h"

class Geometry {
public:
    Geometry() {}
    Geometry(Material *material_ptr) : materialP(material_ptr) {}
    virtual bool intersect(Ray &ray, double &tmin, Shade &sr) = 0;
    virtual bool shadowIntersect(Ray &ray, double &tmin) = 0;
    virtual ~Geometry() {}

    inline Material *getMaterial() {
        return materialP;
    }

    inline void toggleShadowCast(bool shadow) {
        _castShadow = shadow;
    }

    inline bool castShadow() {
        return _castShadow;
    }

protected:
    Material *materialP;
    bool _castShadow = true;
    static constexpr double EPSILON = 1e-6;
};

