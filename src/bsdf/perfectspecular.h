#pragma once

#include "core/brdf.h"

class PerfectSpecular : public BRDF {
public:
    PerfectSpecular();
    PerfectSpecular(float intensity, vec3 color = vec3(1.0));
    virtual vec3 sampleBRDF(Shade &shade, dvec3 &in, const dvec3 &out, float *pdf = nullptr);
    virtual vec3 getColor(Shade &shade) { return color; }

    inline void setIntensity(float value) {
        intensity = value;
    }

    inline void setColor(vec3 value) {
        color = value;
    }

protected:
    float intensity;
    vec3 color;
};