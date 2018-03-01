#include "nrooks.h"
#include "utilities.h"

NRooks::NRooks(int nspl, int nset) : Sampler(nspl, nset) {
    generateSamples();
    setupShuffledIndices();
}

void NRooks::generateSamples() {
    Random rand;
    for (int p = 0; p < numSets; p++)
        for (int j = 0; j < numSamples; j++) {
            double x = (j + rand.randomDouble()) / numSamples;
            double y = (j + rand.randomDouble()) / numSamples;
            samples.push_back(dvec2(x, y));
        }

    shuffleCoordX();
    shuffleCoordY();
}

void NRooks::shuffleCoordX() {
    Random rand;
    for (int p = 0; p < numSets; p++)
        for (int i = 0; i < numSamples - 1; i++) { // only swap (numSamples - 1) times
            int target = rand.randomInteger() % numSamples + p * numSamples;
            swap(samples[p * numSamples + i + 1].x, samples[target].x);
        }
}

void NRooks::shuffleCoordY() {
    Random rand;
    for (int p = 0; p < numSets; p++)
        for (int j = 0; j < numSamples - 1; j++) { // only swap (numSamples - 1) times
            int target = rand.randomInteger() % numSamples + p * numSamples;
            swap(samples[p * numSamples + j + 1].y, samples[target].y);
        }
}
