#pragma once
#include <glm/glm.hpp>
using namespace glm;

class World; // declared first, header is included in source file
struct Shade {
public:
    bool hasHit;
    dvec4 hitPoint;
    dvec3 normal;
    vec3 color;
    World &world;

    Shade(World &wrd) : hasHit(false), hitPoint(), normal(), color(vec3()), world(wrd) {}
    Shade(const Shade &shade);

    Shade & operator = (const Shade &shade);
};

