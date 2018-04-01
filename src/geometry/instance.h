#pragma once

#include "core/geometry.h"

class Instance : public Geometry {
public:
    Instance() {}
    Instance(Geometry *obj_ptr) : objectP(obj_ptr) {}
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    Instance * transform(dmat4 matrix);
    Instance * translate(dvec3 dir);
    Instance * rotate(dvec3 axis, double angle);
    Instance * scale(dvec3 scale);

    virtual void setMaterial(Material *material_ptr) {
        objectP->setMaterial(material_ptr);
    }
    
    virtual ~Instance() {
        if (objectP) delete objectP;
    }

    inline Geometry *getObject() { return objectP; }

private:
    Geometry *objectP = nullptr;
    dmat4 matrix = IDENTITY_MATRIX, invMat;
    dmat3 normalMat;
};