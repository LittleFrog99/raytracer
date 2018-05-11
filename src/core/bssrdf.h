#pragma once

#include "utilities.h"
#include "core/shade.h"
#include "core/sampler.h"

class BSSRDF {
public:
    BSSRDF(float eta) : eta(eta) {}
    virtual vec3 sampleS(Shade &shade, dvec3 &pi, dvec3 &wi) = 0;
    virtual ~BSSRDF() {
        if (dirSmplr) delete dirSmplr;
        if (posSmplr) delete posSmplr;
    }
    
protected:
    float eta;
    Sampler *dirSmplr = nullptr, *posSmplr = nullptr;
};