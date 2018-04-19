#pragma once

#include "core/brdf.h"

class Specular : public BRDF {
public:
    static constexpr float DEFAULT_SPECULAR_EXPONENT = 16.0f;

    Specular(float intensity, vec3 color = Color::WHITE, float exponent = DEFAULT_SPECULAR_EXPONENT);
    virtual vec3 calcBRDF(Shade &shade, dvec3 &in, dvec3 &out);
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out);
    virtual vec3 sampleBRDF(Shade &shade, dvec3 &in, dvec3 &out, float *pdf = (float *)nullptr);
    virtual vec3 getColor(Shade &shade) { return color; }

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