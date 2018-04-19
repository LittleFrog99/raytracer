#include "meshtriangle.h"

void MeshTriangle::setVertex(unsigned int index0, unsigned int index1, unsigned int index2) {
    index = ivec3(index0, index1, index2);
    Triangle::setParams(meshP->vertices[index0].position, 
                        meshP->vertices[index1].position, 
                        meshP->vertices[index2].position);
}

bool MeshTriangle::intersect(Ray &ray, double &tmin, Shade &shade) {
    double t, beta, gamma;
    if (!commonIntersect(ray, t, beta, gamma)) 
        return false;

    tmin = t;
    shade.localHitPoint = ray.origin + t * ray.direction;
    shade.hitPoint = shade.localHitPoint;
    shade.normal = normal;
    shade.texCoord = interpolateUV(beta, gamma);
    return true;
}

vec2 MeshTriangle::interpolateUV(float beta, float gamma) {
    vec2 uv1 = (1 - beta - gamma) * meshP->vertices[index[0]].texCoords;
    vec2 uv2 = beta * meshP->vertices[index[1]].texCoords;
    vec2 uv3 = gamma * meshP->vertices[index[2]].texCoords;
    return normalize(uv1 + uv2 + uv3);
}
