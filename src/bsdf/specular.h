#pragma once

#include "core/brdf.h"

class Specular : public BRDF {
public:
    Specular(float intensity = 0.0, vec3 color = vec3(), float exponent = 16.0f);
    virtual vec3 calcBRDF(Shade &shade, dvec3 &in, dvec3 &out);
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out);
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *pdf = (float *)nullptr);

    inline void setIntensity(float value) {
        intensity = value;
    }

    inline void setColor(vec3 color) {
        this->color = color;
    }
    
    inline void setExponent(float exponent) {
        this->exponent = exponent;
        samplerP->mapSamplesToHemisphere(exponent);
    }

    inline float getExponent() {
        return exponent;
    }

protected:
    float intensity;
    vec3 color;
    float exponent;
};