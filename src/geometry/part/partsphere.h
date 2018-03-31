#pragma once

#include "geometry/primitive/sphere.h"

class PartSphere : public Sphere {
public:
    PartSphere() {}
    PartSphere(Material *mat_ptr) : Sphere(mat_ptr) {}
    void setParams(dvec3 center, double radius, double azim_min, 
                   double azim_max, double polar_min, double polar_max);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);

private:
    double azimMin, azimMax, polarMin, polarMax;
    double cosPolarMin, cosPolarMax;

    bool inRange(dvec3 &dir);
};