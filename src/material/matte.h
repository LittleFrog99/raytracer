#pragma once

#include "core/material.h"
#include "bsdf/lambertian.h"
#include "sampler/multijittered.h"

class Matte : public Material {
public:
    Matte(vec3 color = vec3(), float ambient_factor = 0.0, float diffuse_factor = 0.0);
    virtual vec3 shade(Shade &shade);
    virtual vec3 pathShade(Shade &shade);
    virtual vec3 photonShade(Shade &shade);
    virtual void photonInteract(Shade &shade, PhotonMap *map, Photon *photon);
    virtual ~Matte();

    inline void setAmbientIntensity(float factor) {
        ambBRDF->setIntensity(factor);
    }
    inline void setDiffuseIntensity(float factor) {
        diffBRDF->setIntensity(factor);
    }
    inline void setDiffuseColor(vec3 color) {
        ambBRDF->setColor(color);
        diffBRDF->setColor(color);
    }

protected:
    Lambertian *ambBRDF;
    Lambertian *diffBRDF;
};
