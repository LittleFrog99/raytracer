#pragma once

#include "core/geometry.h"

class Compound : public Geometry {
public:
    Compound() {}
    virtual void setMaterial(Material *material_ptr);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    virtual ~Compound();

    inline void addObject(Geometry *object_ptr) {
        objects.push_back(object_ptr);
    }

protected:
    vector<Geometry*> objects;
};