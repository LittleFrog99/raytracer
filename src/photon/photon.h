#pragma once

#include "utilities.h"

struct Photon {
    dvec3 position;
    double azim, polar;
    vec3 power;
    short dim = 0;
    short specBounce = 0, diffBounce = 0;

    dvec3 getDirection();
    void setDirection(dvec3 dir);

    bool isCaustic() { return specBounce > 0 && diffBounce == 0; }
    inline short totalBounce() { return specBounce + diffBounce; }
};

/* struct Photon {
    dvec3 position;
    double azim, polar;
    vec3 power;
    short bounce;

    Photon(dvec3 position, dvec3 direction, vec3 power, short bounce = 0);
    dvec3 getDirection();
    void output();
};*/