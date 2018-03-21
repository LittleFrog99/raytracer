#pragma once

#include "core/sampler.h"

class PureRandom : public Sampler {
public:
    PureRandom(int nspl, int nset);
private:
    void generateSamples();
};
