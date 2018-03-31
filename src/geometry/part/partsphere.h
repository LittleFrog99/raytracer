#pragma once

#include "geometry/primitive/sphere.h"

class PartSphere : public Sphere {
public:
    PartSphere() {}
    PartSphere(Material *mat_ptr) : Sphere(mat_ptr) {}
    void setParams(dvec3 center, double radius, double theta_min, 
                   double theta_max, double phi_min, double phi_max);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);

private:
    double thetaMin, thetaMax, phiMin, phiMax;
    double cosThetaMin, cosThetaMax;

    bool inRange(dvec3 dir);
};