#pragma once

#include "utilities.h"
#include "core/material.h"
#include "core/ray.h"

class World; // declared first, header is included in source file
struct Shade {
public:
    bool hasHit = false;
    Material *materialP = nullptr;
    dvec3 hitPoint;
    dvec3 localHitPoint;
    dvec3 normal;
    vec3 color;
    Ray ray;
    double t;
    int depth = 0;
    World &world;

    Shade(World &wrd) : world(wrd) {}
};

