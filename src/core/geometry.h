#pragma once

#include "core/shade.h"
#include "core/ray.h"
#include "core/material.h"
#include "core/sampler.h"

class Geometry {
public:
    Geometry() {}
    Geometry(Material *material_ptr) : materialP(material_ptr) {}
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade) { return false; }
    virtual bool shadowIntersect(Ray &ray, double &tmin) { return false; }
    virtual dvec3 sample() { return vec3(); }
    virtual float probDensity(Shade &shade) { return 1.0; }
    virtual dvec3 getNormal(dvec3 &point) { return vec3(); }
    virtual void setSampler(Sampler *sampler_ptr) {
        if (samplerP) delete samplerP;
        samplerP = sampler_ptr;
    }
    virtual ~Geometry() {}

    inline Material *getMaterial() {
        return materialP;
    }
    inline void setMaterial(Material *material_ptr) {
        materialP = material_ptr;
    }
    inline void toggleShadowCast(bool shadow) {
        _castShadow = shadow;
    }
    inline bool castShadow() {
        return _castShadow;
    }
    

protected:
    Material *materialP = nullptr;
    Sampler *samplerP = nullptr;
    bool _castShadow = true;
    static constexpr double EPSILON = 1e-6;
};

