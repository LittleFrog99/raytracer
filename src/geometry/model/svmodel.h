#pragma once

#include "model.h"

class SVModel : public Model {
public:
    SVModel(string path, TriangleMode mode = SMOOTH);
    static void setMaterialParams(float amb_int, float diff_int, float spec_int);

private:
    static float kAmbient, kDiffuse, kSpecular;

    virtual Mesh * processMesh(aiMesh *mesh, const aiScene *scene);
    virtual void setupGrids(TriangleMode mode);
    ImageTexture * loadTexture(aiMaterial *mat, aiTextureType type);
};