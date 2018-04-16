#pragma once

#include "core/material.h"
#include "bsdf/lambertian.h"
#include "bsdf/specular.h"

class Phong : public Material {
public:
    static constexpr float DEFAULT_SPECULAR_EXPONENT = 16.0f;

    Phong(vec3 color, float amb_int, float diff_int, float spec_int);
    virtual vec3 shade(Shade &shade);
    virtual vec3 globalShade(Shade &shade);
    virtual ~Phong();

    inline void setAmbientIntensity(float intensity) {
        ambBRDF->setIntensity(intensity);
    }

    inline void setDiffuseIntensity(float intensity) {
        diffBRDF->setIntensity(intensity);
    }

    inline void setDiffuseColor(vec3 color) {
        ambBRDF->setColor(color);
        diffBRDF->setColor(color);
    }

    inline void setDiffuseSampler(Sampler *sampler_ptr) {
        diffBRDF->setSampler(sampler_ptr);
    }

    inline void setSpecularIntensity(float intensity) {
        specBRDF->setIntensity(intensity);
    }

    inline void setSpecularColor(vec3 color) {
        specBRDF->setColor(color);
    }

    inline void setSpecularExponent(float exponent) {
        specBRDF->setExponent(exponent);
    }

    inline void setSpecularSampler(Sampler *sampler_ptr) {
        specBRDF->setSampler(sampler_ptr, specBRDF->getExponent());
    }

protected:
    Lambertian *ambBRDF;
    Lambertian *diffBRDF;
    Specular *specBRDF;
};