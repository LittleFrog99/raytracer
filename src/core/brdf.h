#pragma once

#include "core/sampler.h"
#include "core/shade.h"
#include "utilities.h"

class BRDF {
public:
    BRDF(Sampler *sampler = nullptr) : samplerP(sampler) {}
    virtual vec3 calcBRDF(Shade &shade, dvec3 &in, dvec3 &out) { return vec3(); }
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *pdf = nullptr) { return vec3(); }
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out) { return vec3(); };
    virtual ~BRDF() { if (samplerP) delete samplerP; }
    
    inline void setSampler(Sampler *sampler_ptr, float exp = 1.0f) {
        samplerP = sampler_ptr;
        samplerP->mapSamplesToHemisphere(exp);
    } 

protected:
    Sampler *samplerP = nullptr;
};