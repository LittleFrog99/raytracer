#pragma once

#include "utilities.h"

class Mesh {
public:
    vector<dvec3> vertices;
    vector<int> indices;
    vector<dvec3> normals;
    vector<vector<int>> vertexFaces;
    vector<float> u, v;
    int numVertices;
    int numTriangles;
};