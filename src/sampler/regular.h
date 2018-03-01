#pragma once

#include "core/sampler.h"

class Regular : public Sampler {
public:
    Regular(int nspl, int nset = 1);
private:
    void generateSamples();
};
