#pragma once

#include "geometry/model/meshtriangle.h"

class SmoothMeshTriangle : public MeshTriangle {
public:
    SmoothMeshTriangle(Mesh *mesh_ptr, Material *mat_ptr = nullptr) : MeshTriangle(mesh_ptr, mat_ptr) {}
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);

private:
    dvec3 interpolateNormal(double beta, double gamma);
};