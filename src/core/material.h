#pragma once

#include "utilities.h"
struct Shade;

class Material {
public:
    virtual vec3 shade(Shade &shade) { return vec3(); }
};
