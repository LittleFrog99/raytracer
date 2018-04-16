#pragma once

#include "core/sampler.h"
#include "core/shade.h"
#include "utilities.h"

class BTDF {
public:
    virtual vec3 calcBTDF(Shade &shade, dvec3 &in, dvec3 &out) { return vec3(); }
    virtual vec3 sampleBTDF(Shade &shade, dvec3 &trans, dvec3 &out) = 0;
    virtual vec3 calcReflectance(Shade &shade, dvec3 &out) { return vec3(); };
    virtual bool isTIR(Shade &shade) = 0;
    virtual ~BTDF() { if (samplerP) delete samplerP; }

    inline void setSampler(Sampler *sampler_ptr, float exp = 1.0f) {
        samplerP = sampler_ptr;
        samplerP->mapSamplesToHemisphere(exp);
    } 

protected:
    Sampler *samplerP = nullptr;
};