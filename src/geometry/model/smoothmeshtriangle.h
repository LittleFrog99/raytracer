#pragma once

#include "geometry/model/meshtriangle.h"

class SmoothMeshTriangle : public MeshTriangle {
public:
    SmoothMeshTriangle() {}
    SmoothMeshTriangle(Mesh *mesh_ptr) : MeshTriangle(mesh_ptr) {}
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);

private:
    dvec3 interpolateNormal(double beta, double gamma);
};