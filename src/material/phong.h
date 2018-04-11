#pragma once

#include "core/material.h"
#include "bsdf/lambertian.h"
#include "bsdf/specular.h"

class Phong : public Material {
public:
    Phong(vec3 color = vec3(), float ambient_intensity = 0.0, float diffuse_intensity = 0.0, 
    float specular_intensity = 0.0);
    virtual vec3 shade(Shade &shade);
    virtual vec3 globalShade(Shade &shade);
    virtual ~Phong();

    inline void setAmbientIntensity(float intensity) {
        ambientBRDF->setIntensity(intensity);
    }

    inline void setDiffuseIntensity(float intensity) {
        diffuseBRDF->setIntensity(intensity);
    }

    inline void setDiffuseColor(vec3 color) {
        ambientBRDF->setColor(color);
        diffuseBRDF->setColor(color);
    }

    inline void setDiffuseSampler(Sampler *sampler_ptr) {
        diffuseBRDF->setSampler(sampler_ptr);
    }

    inline void setSpecularIntensity(float intensity) {
        specularBRDF->setIntensity(intensity);
    }

    inline void setSpecularColor(vec3 color) {
        specularBRDF->setColor(color);
    }

    inline void setSpecularExponent(float exponent) {
        specularBRDF->setExponent(exponent);
    }

    inline void setSpecularSampler(Sampler *sampler_ptr) {
        specularBRDF->setSampler(sampler_ptr, specularBRDF->getExponent());
    }

protected:
    Lambertian *ambientBRDF;
    Lambertian *diffuseBRDF;
    Specular *specularBRDF;
};