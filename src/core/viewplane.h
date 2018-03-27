#pragma once

#include "core/sampler.h"

struct ViewPlane {
    int horRes;
    int vertRes;
    int numSamples;
    int numChannels;
    double pixelSize;
    float gamma;
    float invGamma; // 1/gamma
    Sampler *samplerP = nullptr;

    void setSampler(Sampler *sampler_ptr);
    void setSamples(const int num_sampler, const int num_sets);
    ~ViewPlane() {}

    inline ivec2 toPixelCoord(int index) {
        return ivec2(index / horRes, index % horRes);
    }
    inline int toIndex(ivec2 coord) {
        return horRes * coord.x + coord.y;
    }
};

