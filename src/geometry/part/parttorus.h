#pragma once

#include "geometry/primitive/torus.h"

class PartTorus : public Torus {
public:
    PartTorus() {}
    PartTorus(Material *mat_ptr) : Torus(mat_ptr) {}
    void setParams(double swept_r, double tube_r, double azim_min, double azim_max,
                   double polar_min, double polar_max);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);

private:
    double azimMin, azimMax; // polar angle
    double polarMin, polarMax; // azimuth angle

    bool inRange(dvec3 &point);
};