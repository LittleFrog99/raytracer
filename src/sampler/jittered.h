#pragma once

#include "core/sampler.h"

class Jittered : public Sampler {
public:
    Jittered(int nspl, int nset);
private:
    void generateSamples();
};
