#pragma once

#include "core/sampler.h"

class Regular : public Sampler {
public:
    Regular(int num);
private:
    void generateSamples();
};
