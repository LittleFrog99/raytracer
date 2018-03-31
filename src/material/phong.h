#pragma once

#include "core/material.h"
#include "brdf/lambertian.h"
#include "brdf/specular.h"

class Phong : public Material {
public:
    Phong(vec3 color = vec3(), float ambient_intensity = 0.0, float diffuse_intensity = 0.0, 
    float specular_intensity = 0.0);
    virtual vec3 shade(Shade &shade);
    virtual vec3 areaLightShade(Shade &shade);

    inline void setAmbientIntensity(float intensity) {
        ambientBRDF->setIntensity(intensity);
    }
    inline void setDiffuseIntensity(float intensity) {
        diffuseBRDF->setIntensity(intensity);
    }
    inline void setSpecularIntensity(float intensity) {
        specularBRDF->setIntensity(intensity);
    }
    inline void setDiffuseColor(vec3 color) {
        ambientBRDF->setColor(color);
        diffuseBRDF->setColor(color);
    }
    inline void setSpecularColor(vec3 color) {
        specularBRDF->setColor(color);
    }
    inline void setSpecularExponent(float exponent) {
        specularBRDF->setExponent(exponent);
    }

private:
    Lambertian *ambientBRDF;
    Lambertian *diffuseBRDF;
    Specular *specularBRDF;
};