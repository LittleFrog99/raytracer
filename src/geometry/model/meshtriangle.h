#pragma once

#include "geometry/primitive/triangle.h"
#include "geometry/model/mesh.h"

class MeshTriangle : public Triangle {
public:
    MeshTriangle() {}
    MeshTriangle(Mesh *mesh_ptr) : meshP(mesh_ptr) {}
    void setVertex(int index0, int index1, int index2);

protected:
    Mesh *meshP;
    ivec3 index;
};