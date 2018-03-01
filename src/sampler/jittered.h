#pragma once

#include "core/sampler.h"

class Jittered : public Sampler {
public:
    Jittered(int num);
private:
    void generateSamples();
};
