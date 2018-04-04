#include "triangle.h"

void Triangle::setParams(dvec3 vert_a, dvec3 vert_b, dvec3 vert_c) {
    vertA = vert_a, vertB = vert_b, vertC = vert_c;
    normal = normalize(cross(vert_b - vert_a, vert_c - vert_a));

    dmat3 trans = transpose(dmat3(vertA, vertB, vertC));
    using namespace Math;
    dvec3 minBnd = dvec3(minComponent(trans[0]), minComponent(trans[1]), minComponent(trans[2]));
    dvec3 maxBnd = dvec3(maxComponent(trans[0]), maxComponent(trans[1]), maxComponent(trans[2]));
    createBoundingBox(minBnd, maxBnd);
}

bool Triangle::intersect(Ray &ray, double &tmin, Shade &shade) {
    dvec3 colA = vertA - vertB, colB = vertA - vertC, 
          colC = ray.direction, colD = vertA - ray.origin;
    double denom = determinant(dmat3(colA, colB, colC));

    double beta = determinant(dmat3(colD, colB, colC)) / denom;
    if (beta < 0.0) return false;
    double gamma = determinant(dmat3(colA, colD, colC)) / denom;
    if (gamma < 0.0) return false;
    if (beta + gamma > 1.0) return false;

    double t = determinant(dmat3(colA, colB, colD)) / denom;
    if (t < EPSILON) return false;

    tmin = t;
    shade.normal = normal;
    shade.localHitPoint = ray.origin + t * ray.direction;
    shade.hitPoint = shade.localHitPoint;
    return true;
}

bool Triangle::shadowIntersect(Ray &ray, double &tmin) {
    dvec3 colA = vertA - vertB, colB = vertA - vertC, 
          colC = ray.direction, colD = vertA - ray.origin;
    double denom = determinant(dmat3(colA, colB, colC));

    double beta = determinant(dmat3(colD, colB, colC)) / denom;
    if (beta < 0.0) return false;
    double gamma = determinant(dmat3(colA, colD, colC)) / denom;
    if (gamma < 0.0) return false;
    if (beta + gamma > 1.0) return false;

    double t = determinant(dmat3(colA, colB, colD)) / denom;
    if (t < EPSILON) return false;

    tmin = t;
    return true;
}