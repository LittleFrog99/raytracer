#pragma once

#include "core/brdf.h"

class GlossySpecular : public BRDF {
public:
    GlossySpecular(Sampler *sampler = nullptr, float intensity = 0.0, vec3 color = vec3(), 
        float exponent = 16.0f) : BRDF(sampler), intensity(intensity), color(color), 
        exponent(exponent) {}
    virtual vec3 calcBRDF(Shade &shade, dvec3 &in, dvec3 &out);
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *prob_den = nullptr);
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out);

    inline void setIntensity(float value) {
        intensity = value;
    }
    inline void setColor(vec3 color) {
        this->color = color;
    }
    inline void setExponent(float exponent) {
        this->exponent = exponent;
    }

private:
    float intensity;
    vec3 color;
    float exponent;
};