#pragma once

#include "core/brdf.h"

class Lambertian : public BRDF {
public:
    Lambertian(float intensity = 0, vec3 color = vec3());
    virtual vec3 calcBRDF(Shade &shade, dvec3 &in, dvec3 &out);
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *pdf = nullptr);
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out);
    virtual ~Lambertian() { if (samplerP) delete samplerP; }

    inline void setIntensity(float factor) {
        intensity = factor;
    }

    inline void setColor(vec3 color) {
        this->color = color;
    }

private:
    float intensity;
    vec3 color;
};
