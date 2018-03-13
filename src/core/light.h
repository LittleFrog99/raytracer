#pragma once

#include "utilities.h"
#include "core/shade.h"

class Light {
public:
    Light() {}
    virtual dvec3 getDirection(Shade &shade) = 0;
    virtual vec3 incidRadiosity(Shade &shade) = 0;

protected:
    bool castShadow = true;
};