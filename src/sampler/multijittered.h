#pragma once

#include "core/sampler.h"

class MultiJittered : public Sampler {
public:
    MultiJittered(int nspl, int nset);
    ~MultiJittered() {}
private:
    void generateSamples();
};
