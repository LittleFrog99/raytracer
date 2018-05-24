#pragma once

#include "core/shade.h"
#include "core/ray.h"
#include "core/material.h"
#include "core/sampler.h"
#include "geometry/bound/boundingbox.h"

class Geometry {
public:
    Geometry() {}
    Geometry(Material *material_ptr) : materialP(material_ptr) {}
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade) = 0;
    virtual bool shadowIntersect(Ray &ray, double &tmin) = 0;
    virtual dvec3 getNormal(dvec3 &point) { return vec3(); }
    virtual dvec3 sample() { return vec3(); }
    virtual float calcPdf(Shade &shade) { return 1.0; }
    virtual void setSampler(Sampler *sampler_ptr);
    BoundingBox getBoundingBox();
    virtual void setMaterial(Material *material_ptr) { materialP = material_ptr;}
    virtual ~Geometry() {}

    inline Material *getMaterial() { return materialP; }
    inline void toggleShadowCast(bool shadow) { _castShadow = shadow; }
    inline bool castShadow() { return _castShadow; }

protected:
    Material *materialP = nullptr;
    Sampler *samplerP = nullptr;
    bool _castShadow = true;

    /* for geometric objects that does not inherit from BoxBounded */
    virtual BoundingBox calcBoundingBox() {  
        return BoundingBox(dvec3(), dvec3()); 
    }
};
