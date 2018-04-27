#pragma once

#include "utilities.h"

struct Photon {
    dvec3 position;
    double azim, polar;
    vec3 power;
    short bounce;

    Photon(dvec3 position, dvec3 direction, vec3 power, short bounce = 0);
    dvec3 getDirection();
    void output();
};