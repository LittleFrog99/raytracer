#pragma once

#include "core/sampler.h"
#include "core/shade.h"
#include "utilities.h"

class BRDF {
public:
    BRDF(Sampler *sampler = nullptr) : samplerP(sampler) {}
    virtual vec3 calcBRDF(Shade &shade, dvec3 &in, dvec3 &out) = 0;
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *prob_den = nullptr) = 0;
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out) = 0;
    virtual ~BRDF() { if (samplerP) delete samplerP; }
    
    inline void setSampler(Sampler *sampler_ptr, float exp) {
        samplerP = sampler_ptr;
        samplerP->mapSamplesToHemisphere(exp);
    }
    

protected:
    Sampler *samplerP = nullptr;
};