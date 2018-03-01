#pragma once

#include "core/sampler.h"

class MultiJittered : public Sampler {
public:
    MultiJittered(int num);
private:
    void generateSamples();
};
