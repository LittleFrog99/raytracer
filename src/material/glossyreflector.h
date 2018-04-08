#pragma once

#include "material/phong.h"
#include "brdf/glossyspecular.h"

class GlossyReflector : public Phong {
public:
    GlossyReflector() {}
    GlossyReflector(vec3 color = vec3(), float amb_int = 0.0, float diff_int = 0.0, 
               float spec_int = 0.0, float refl_int = 0.0);
    virtual vec3 shade(Shade &shade);

    inline void setGlossyReflectionIntensity(float value) {
        glossySpecularBRDF->setIntensity(value);
    }

    inline void setGlossyReflectionColor(vec3 color) {
        glossySpecularBRDF->setColor(color);
    }

    inline void setGlossyReflectionExponent(float exponent) {
        glossySpecularBRDF->setExponent(exponent);
    }

    inline void setGlossyReflectionSampler(Sampler *sampler_ptr) {
        glossySpecularBRDF->setSampler(sampler_ptr, glossySpecularBRDF->getExponent());
    }

private:
    GlossySpecular *glossySpecularBRDF;
};