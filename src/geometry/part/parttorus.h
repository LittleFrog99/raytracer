#pragma once

#include "geometry/primitive/torus.h"

class PartTorus : public Torus {
public:
    PartTorus() {}
    PartTorus(Material *mat_ptr) : Torus(mat_ptr) {}
    void setParams(double swept_r, double tube_r, double phi_min, double phi_max,
                   double theta_min, double theta_max);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);

private:
    double phiMin, phiMax;
    double thetaMin, thetaMax;

    bool inRange(dvec3 &point);
};