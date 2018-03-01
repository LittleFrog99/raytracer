#include "purerandom.h"
#include "utilities.h"

PureRandom::PureRandom(int nspl, int nset) : Sampler(nspl, nset) {
    generateSamples();
    setupShuffledIndices();
}

void PureRandom::generateSamples() {
    Random rand;
    for (int i = 0; i < numSets; i++)
         for (int j = 0; j < numSamples; j++)
             samples.push_back(dvec2(rand.randomDouble(), rand.randomDouble()));
}
