#pragma once

#include "core/material.h"
#include "brdf/lambertian.h"
#include "sampler/multijittered.h"

class Matte : public Material {
public:
    Matte(vec3 color = vec3(), float ambient_factor = 0.0, float diffuse_factor = 0.0);
    virtual vec3 shade(Shade &shade);

    inline void setAmbientIntensity(float factor) { 
        ambientBRDF->setIntensity(factor);
    }
    inline void setDiffuseIntensity(float factor) {
        diffuseBRDF->setIntensity(factor);
    }
    inline void setDiffuseColor(vec3 color) {
        ambientBRDF->setColor(color);
        diffuseBRDF->setColor(color);
    }

private:
    Lambertian *ambientBRDF;
    Lambertian *diffuseBRDF;
};
