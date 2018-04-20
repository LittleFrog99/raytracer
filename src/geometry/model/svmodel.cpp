#include "svmodel.h"
#include "material/svmaterial.h"
#include "geometry/model/meshtriangle.h"
#include "geometry/model/smoothmeshtriangle.h"

float SVModel::kAmbient = 0.3;
float SVModel::kDiffuse = 0.5;
float SVModel::kSpecular = 0.3;

SVModel::SVModel(string path, TriangleMode mode) {
    loadModel(path);
    setupGrids(mode);
}

void SVModel::setMaterialParams(float amb_int, float diff_int, float spec_int) {
    kAmbient = amb_int;
    kDiffuse = diff_int;
    kSpecular = spec_int;
}

Mesh * SVModel::processMesh(aiMesh *mesh, const aiScene *scene) {
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

    map<TextureType, ImageTexture *> textures; // load textures
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    textures[DIFFUSE] = loadTexture(material, aiTextureType_DIFFUSE);

    return new Mesh(vertices, indices, textures, mesh->mNumVertices, mesh->mNumFaces);
}

ImageTexture * SVModel::loadTexture(aiMaterial *mat, aiTextureType type) {
    aiString path;
    mat->GetTexture(type, 0, &path);
    return new ImageTexture(directory + '/' + path.data, MESHUV);
}

void SVModel::setupGrids(Model::TriangleMode mode) {
    for (Mesh *mesh : meshes) {
        Material *matP = new SVPhong(mesh->textures[DIFFUSE], kAmbient, kDiffuse, kSpecular);
        for (int i = 0; i < mesh->numFaces; i++) {
            MeshTriangle *triangle;
            if (mode == SMOOTH)
                triangle = new SmoothMeshTriangle(mesh, matP);
            else 
                triangle = new MeshTriangle(mesh, matP);

            triangle->setVertex(mesh->indices[3*i], mesh->indices[3*i+1], mesh->indices[3*i+2]);
            objects.push_back(triangle);
        }
    }
    Grid::setupCells();
}
