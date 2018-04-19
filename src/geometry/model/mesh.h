#pragma once

#include "material/texture/imagetexture.h"
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
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, int num_vertices, int num_faces)
        : vertices(vertices), indices(indices), numVertices(num_vertices), numFaces(num_faces) {}
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, map<TextureType, ImageTexture *> textures,
         int num_vertices, int num_faces)
        : vertices(vertices), indices(indices), textures(textures), numVertices(num_vertices),
          numFaces(num_faces) {}
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    map<TextureType, ImageTexture *> textures;
    int numVertices;
    int numFaces;

    ~Mesh() {
        vertices.clear();
        indices.clear();
        for (auto pair : textures) 
            delete pair.second;
        textures.clear();
    }
};