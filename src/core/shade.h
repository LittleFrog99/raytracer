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
    vec2 texCoord;
    vec3 color;
    Ray ray;
    double t;
    int depth = 0;
    World &world;

    Shade(World &wrd) : world(wrd) {}
    Shade & operator = (Shade &shade) {
        hasHit = shade.hasHit;
        materialP = shade.materialP;
        hitPoint = shade.hitPoint;
        localHitPoint = shade.localHitPoint;
        normal = shade.normal;
        texCoord = shade.texCoord;
        color = shade.color;
        ray = shade.ray;
        t = shade.t;
        depth = shade.depth;
        // world reference initialized cannot be assigned
        return *this;
    }
};

