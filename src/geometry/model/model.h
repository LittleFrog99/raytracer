#pragma once

#include "geometry/model/mesh.h"
#include "geometry/compound/grid.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Grid {
public:
    enum TriangleMode { SMOOTH, FLAT};
    
    Model(string path, Material *mat_ptr, TriangleMode mode = SMOOTH);
    ~Model();

private:
    vector<Mesh *> meshes;
    string directory;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh * processMesh(aiMesh *mesh, const aiScene *scene);
    dvec3 convertVector(aiVector3D &vec);
    vec2 convertVector(aiVector2D &vec);
    void setupGrids(TriangleMode mode);
};