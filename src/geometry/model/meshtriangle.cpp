#include "meshtriangle.h"

void MeshTriangle::setVertex(int index0, int index1, int index2) {
    index = ivec3(index0, index1, index2);
    Triangle::setParams(meshP->vertices[index0].position, 
                        meshP->vertices[index1].position, 
                        meshP->vertices[index2].position);
}
