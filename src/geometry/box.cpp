#include "box.h"

void Box::setParams(dvec3 vertex1, dvec3 vertex2) {
    vertMin = vertex1, vertMax = vertex2;
    for (int i = 0; i < 3; i++)
        if (vertMin[i] > vertMax[i])
            swap(vertMin[i], vertMax[i]);
}

bool Box::intersect(Ray &ray, double &t_min, Shade &shade) {
    dvec3 tMin, tMax;
    for (int i = 0; i < 3; i++) {
        double component = ray.direction[i];
        tMin[i] = (component >= 0 ? (vertMin[i] - ray.origin[i])
                                  : (vertMax[i] - ray.origin[i])) / component;
        tMax[i] = (component >= 0 ? (vertMax[i] - ray.origin[i])
                                  : (vertMin[i] - ray.origin[i])) / component;
    }

    Face faceIn, faceOut;
    int inIndex, outIndex;
    double tIn = Math::maxComponent(tMin, inIndex), 
           tOut = Math::minComponent(tMax, outIndex);
    faceIn = Face(inIndex + 3 * (ray.direction[inIndex] < 0));
    faceOut = Face(outIndex + 3 * (ray.direction[outIndex] >= 0));

    if (tIn < tOut && tOut > EPSILON) {
        if (tIn > EPSILON) {
            t_min = tIn;
            shade.normal = getNormal(faceIn);
        } else {
            t_min = tOut;
            shade.normal = getNormal(faceOut);
        }
        shade.localHitPoint = ray.origin + t_min * ray.direction;
        shade.hitPoint = shade.localHitPoint;
        return true;
    }
    else return false;
}

dvec3 Box::getNormal(Face face) {
    switch (face) {
       case NEG_X: return dvec3(-1, 0, 0); 
       case NEG_Y: return dvec3(0, -1, 0);
       case NEG_Z: return dvec3(0, 0, -1);
       case POS_X: return dvec3(1, 0, 0);
       case POS_Y: return dvec3(0, 1, 0);
       case POS_Z: return dvec3(0, 0, 1);
    }
}

dvec3 Box::getNormal(dvec3 &point) {
    int maxIndex; 
    bool positive = Math::maxComponent(abs(point), maxIndex) > 0;
    return getNormal(Face(maxIndex + 3 * positive));
}

bool Box::shadowIntersect(Ray &ray, double &t_min) {
    dvec3 tMin, tMax;
    for (int i = 0; i < 3; i++) {
        double component = ray.direction[i];
        tMin[i] = (component >= 0 ? (vertMin[i] - ray.origin[i])
                                  : (vertMax[i] - ray.origin[i])) / component;
        tMax[i] = (component >= 0 ? (vertMax[i] - ray.origin[i])
                                  : (vertMin[i] - ray.origin[i])) / component;
    }

    Face faceIn, faceOut;
    int inIndex, outIndex;
    double tIn = Math::maxComponent(tMin, inIndex), 
           tOut = Math::minComponent(tMax, outIndex);
    faceIn = Face(inIndex + 3 * (ray.direction[inIndex] < 0));
    faceOut = Face(outIndex + 3 * (ray.direction[outIndex] > 0));

    if (tIn < tOut && tOut > EPSILON) {
        t_min = tIn > EPSILON ? tIn : tOut;
        return true;
    }
    else return false;
}