#pragma once

#include "utilities.h"

struct Vertex {
    dvec3 position;
    dvec3 normal;
    vec2 texCoords;
    dvec3 tangent;
    dvec3 biTangent;
};

class Mesh {
public:
    Mesh() {}
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, int num_vertices, int num_faces) :
        vertices(vertices), indices(indices), numVertices(num_vertices), numFaces(num_faces) {}
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    int numVertices;
    int numFaces;
};