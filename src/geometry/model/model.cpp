#include "model.h"
#include "geometry/model/meshtriangle.h"
#include "geometry/model/smoothmeshtriangle.h"

Model::Model(string path, Material *mat_ptr, TriangleMode mode) {
    materialP = mat_ptr;
    loadModel(path);
    setupGrids(mode);
}

void Model::loadModel(string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | 
                                                   aiProcess_FlipUVs | 
                                                   aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh * Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    for (int i = 0; i < mesh->mNumVertices; i++) { // load vertices
        Vertex vert;
        vert.position = convertVector(mesh->mVertices[i]);
        vert.normal = convertVector(mesh->mNormals[i]);
        if (mesh->mTextureCoords[0]) 
            vert.texCoords = vec2(convertVector(mesh->mTextureCoords[0][i]));
        vert.tangent = convertVector(mesh->mTangents[i]);
        vert.biTangent = convertVector(mesh->mBitangents[i]);
        vertices.push_back(vert);
    }

    for (int i = 0; i < mesh->mNumFaces; i++) { // load indices
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) 
            indices.push_back(face.mIndices[j]);
    }

    return new Mesh(vertices, indices, mesh->mNumVertices, mesh->mNumFaces);
}

dvec3 Model::convertVector(aiVector3D &vec) {
    return dvec3(vec.x, vec.y, vec.z);
}

vec2 Model::convertVector(aiVector2D &vec) {
    return vec2(vec.x, vec.y);
}

void Model::setupGrids(TriangleMode mode) {
    for (Mesh *mesh : meshes) {
        for (int i = 0; i < mesh->numFaces; i++) {
            MeshTriangle *triangle;
            if (mode == SMOOTH)
                triangle = new SmoothMeshTriangle(mesh);
            else 
                triangle = new MeshTriangle(mesh);

            triangle->setVertex(mesh->indices[3*i], mesh->indices[3*i+1], mesh->indices[3*i+2]);
            objects.push_back(triangle);
        }
    }
    setMaterial(materialP);
    Grid::setupCells();
}

Model::~Model() {
    for (Mesh *meshP : meshes)
        delete meshP;
}