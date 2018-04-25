#pragma once

#include "utilities.h"
struct Shade;

class Material {
public:
    virtual vec3 getEmissiveLight(Shade &shade) { return vec3(); }
    virtual vec3 shade(Shade &shade) = 0;
    virtual vec3 pathShade(Shade &sr) { return shade(sr); }
    virtual vec3 photonShade(Shade &sr) { return vec3(); }
    virtual ~Material() {}
};
