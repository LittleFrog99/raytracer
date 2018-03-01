#pragma once

#include "core/sampler.h"

class PureRandom : public Sampler {
public:
    PureRandom(int num);
private:
    void generateSamples();
};
