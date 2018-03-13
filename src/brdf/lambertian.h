#pragma once

#include "core/brdf.h"

class Lambertian : public BRDF {
public:
    Lambertian(Sampler *sampler, float diff_factor = 0, vec3 color_factor = vec3()) : 
        BRDF(sampler), reflectance(diff_factor), color(color_factor) {}
    virtual vec3 calcBRDF(Shade &shade, dvec3 &in, dvec3 &out);
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out);
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out, float &reflect);
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out);

    inline void setReflectance(float factor) {
        reflectance = factor;
    }
    inline void setColor(vec3 color) {
        this->color = color;
    }
    virtual ~Lambertian() { if (samplerP) delete samplerP; }

private:
    float reflectance;
    vec3 color;
};
