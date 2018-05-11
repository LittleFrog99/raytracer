#pragma once

#include "core/brdf.h"

class Lambertian : public BRDF {
public:
    Lambertian(float intensity, vec3 color = vec3());
    virtual vec3 calcBRDF(Shade &shade, dvec3 &in, const dvec3 &out);
    virtual vec3 sampleBRDF(Shade &shade, dvec3 &in, const dvec3 &out, float *pdf = nullptr);
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out);
    virtual vec3 getColor(Shade &shade) { return color; }
    virtual ~Lambertian() { if (samplerP) delete samplerP; }

    inline void setIntensity(float factor) { intensity = factor; }
    inline void setColor(vec3 color) { this->color = color; }

    float intensity;
    vec3 color;
};
