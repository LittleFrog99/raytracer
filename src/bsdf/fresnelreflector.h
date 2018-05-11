#pragma once

#include "bsdf/perfectspecular.h"

class FresnelReflector : public PerfectSpecular {
public:
    FresnelReflector(vec3 color, float eta_in, float eta_out);
    virtual vec3 sampleBRDF(Shade &shade, dvec3 &in, const dvec3 &out, float *pdf = nullptr);
    float fresnelReflFactor(Shade &shade);

    inline void setEta(float eta_in, float eta_out) {
        etaIn = eta_in;
        etaOut = eta_out;
    }

private:
    float etaIn, etaOut;
};