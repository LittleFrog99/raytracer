#pragma once

#include "core/sampler.h"

class ViewPlane {
public:
    int horRes;
    int vertRes;
    int numSamples;
    int numChannels;
    double pixelSize;
    float gamma;
    float invGamma; // 1/gamma

    void setSampler(Sampler *sampler_ptr);
    inline Sampler * getSampler() { return samplerP; }
    void setSamples(const int n);
    ~ViewPlane() { delete samplerP; }

private:
    Sampler *samplerP;
};

