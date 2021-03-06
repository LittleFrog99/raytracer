#include "boundingbox.h"

BoundingBox::BoundingBox(dvec3 vertex1, dvec3 vertex2) {
    vertMin = vertex1, vertMax = vertex2;
    for (int i = 0; i < 3; i++) 
        if (vertMin[i] > vertMax[i]) swap(vertMin[i], vertMax[i]);
}

bool BoundingBox::intersect(Ray &ray) {
    dvec3 tMin, tMax;
    for (int i = 0; i < 3; i++) {
        double component = ray.direction[i];
        tMin[i] = (component >= 0 ? (vertMin[i] - ray.origin[i]) 
                                  : (vertMax[i] - ray.origin[i])) / component;
        tMax[i] = (component >= 0 ? (vertMax[i] - ray.origin[i])
                                  : (vertMin[i] - ray.origin[i])) / component;
    }

   double in = Math::maxComp(tMin);
   double out = Math::minComp(tMax);

   return in < out && out > EPSILON;
}

bool BoundingBox::inside(dvec3 &point) {
    return Math::inside(point.x, vertMin.x, vertMax.x) &&
           Math::inside(point.y, vertMin.y, vertMax.y) &&
           Math::inside(point.z, vertMin.z, vertMax.z);
}