#pragma once

#include "core/sampler.h"

class NRooks : public Sampler {
public:
    NRooks(int num);
private:
    void generateSamples();
    void shuffleCoordX();
    void shuffleCoordY();
};
