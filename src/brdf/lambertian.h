#pragma once

#include "core/brdf.h"

class Lambertian : public BRDF {
public:
    Lambertian(Sampler *sampler = nullptr, float intensity = 0, vec3 color = vec3()) : 
        BRDF(sampler), intensity(intensity), color(color) {}
    virtual vec3 calcBRDF(Shade &shade, dvec3 &in, dvec3 &out);
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *prob_den = nullptr);
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out);

    inline void setIntensity(float factor) {
        intensity = factor;
    }
    inline void setColor(vec3 color) {
        this->color = color;
    }
    virtual ~Lambertian() { if (samplerP) delete samplerP; }

private:
    float intensity;
    vec3 color;
};
