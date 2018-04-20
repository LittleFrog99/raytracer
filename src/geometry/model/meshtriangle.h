#pragma once

#include "geometry/primitive/triangle.h"
#include "geometry/model/mesh.h"

class MeshTriangle : public Triangle {
public: 
    MeshTriangle(Mesh *mesh_ptr, Material *mat_ptr = nullptr) : Triangle(mat_ptr), meshP(mesh_ptr) {}
    void setVertex(unsigned int index0, unsigned int index1, unsigned int index2);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);

protected:
    Mesh *meshP;
    ivec3 index;

    vec2 interpolateUV(float beta, float gamma);
};