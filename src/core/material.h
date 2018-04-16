#pragma once

#include "utilities.h"
struct Shade;

class Material {
public:
    virtual vec3 getEmissiveLight(Shade &shade) { return vec3(); }
    virtual vec3 shade(Shade &shade) = 0;
    virtual vec3 globalShade(Shade &sr) { return shade(sr); }
    virtual ~Material() {}
};
