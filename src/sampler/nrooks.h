#pragma once

#include "core/sampler.h"

class NRooks : public Sampler {
public:
    NRooks(int nspl, int nset);
private:
    void generateSamples();
    void shuffleCoordX();
    void shuffleCoordY();
};
