#pragma once

#include "core/material.h"
#include "bsdf/tabulated.h"

class Subsurface : public Material {
public:
    Subsurface(float eta, float g, vec3 absorp, vec3 scatter);
    virtual vec3 pathShade(Shade &po);

private:
    Tabulated *SSS;
    BSSRDFTable table;
};