#include "triangle.h"

void Triangle::setParams(dvec3 vert_a, dvec3 vert_b, dvec3 vert_c) {
    vertA = vert_a, vertB = vert_b, vertC = vert_c;
    dvec3 crossVec = cross(vert_b - vert_a, vert_c - vert_a);
    normal = normalize(crossVec);
    area = length(crossVec);
}

bool Triangle::intersect(Ray &ray, double &tmin, Shade &shade) {
    double t, beta, gamma;
    if (!commonIntersect(ray, t, beta, gamma)) 
        return false;

    tmin = t;
    shade.localHitPoint = ray.origin + t * ray.direction;
    shade.materialP = getMaterial();
    shade.normal = normal;
    return true;
}

bool Triangle::shadowIntersect(Ray &ray, double &tmin) {
    double t, beta, gamma;
    if (!commonIntersect(ray, t, beta, gamma)) 
        return false;

    tmin = t;
    return true;
}

BoundingBox Triangle::calcBoundingBox() {
    using namespace Math;
    dmat3 trans = transpose(dmat3(vertA, vertB, vertC));
    dvec3 minBnd = dvec3(minComp(trans[0]), minComp(trans[1]), minComp(trans[2]));
    dvec3 maxBnd = dvec3(maxComp(trans[0]), maxComp(trans[1]), maxComp(trans[2]));
    return BoundingBox(minBnd - dvec3(EPSILON), maxBnd + dvec3(EPSILON));
}

bool Triangle::commonIntersect(Ray &ray, double &t, double &beta, double &gamma) {
    dvec3 colA = vertA - vertB, colB = vertA - vertC,
          colC = ray.direction, colD = vertA - ray.origin;
    double denom = determinant(dmat3(colA, colB, colC));

    beta = determinant(dmat3(colD, colB, colC)) / denom;
    if (beta < 0.0) 
        return false;
    gamma = determinant(dmat3(colA, colD, colC)) / denom;
    if (gamma < 0.0) 
        return false;
    if (beta + gamma > 1.0) 
        return false;

    t = determinant(dmat3(colA, colB, colD)) / denom;
    if (t < EPSILON) 
        return false;
    else return true;
}