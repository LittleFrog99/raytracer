#pragma once

#include "bsdf/perfecttransmitter.h"

class FresnelTransmitter : public PerfectTransmitter {
public:
    FresnelTransmitter(vec3 color, float eta_in, float eta_out);
    virtual vec3 sampleBTDF(Shade &shade, dvec3 &trans, const dvec3 &out);
    virtual bool isTIR(Shade &shade);
    float fresnelTransFactor(Shade &shade);

    inline void setEta(float eta_in, float eta_out) {
        etaIn = eta_in;
        etaOut = eta_out;
    }

private:
    float etaIn, etaOut;
};