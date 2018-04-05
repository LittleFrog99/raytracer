#include "smoothmeshtriangle.h"
#include "debug.h"

bool SmoothMeshTriangle::intersect(Ray &ray, double &tmin, Shade &shade) {
    double t, beta, gamma;
    if (!Triangle::commonIntersect(ray, t, beta, gamma))
        return false;
    
    tmin = t;
    shade.localHitPoint = ray.origin + t * ray.direction;
    shade.hitPoint = shade.localHitPoint;
    shade.normal = interpolateNormal(beta, gamma);
    return true;
}

dvec3 SmoothMeshTriangle::interpolateNormal(double beta, double gamma) {
    dvec3 normA = (1 - beta - gamma) * meshP->vertices[index[0]].normal;
    dvec3 normB = beta * meshP->vertices[index[1]].normal;
    dvec3 normC = gamma * meshP->vertices[index[2]].normal;
    return normalize(normA + normB + normC);
}
