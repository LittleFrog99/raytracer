#include "jittered.h"
#include "utilities.h"

Jittered::Jittered(int nspl, int nset) : Sampler(nspl, nset) {
    generateSamples();
    setupShuffledIndices();
}

void Jittered::generateSamples() {
    Random rand;
    int num = sqrt(numSamples);

    for (int i = 0; i < numSets; i++)
        for (int j = 0; j < num; j++)
            for (int k = 0; k < num; k++) {
                dvec2 sample((k + rand.randomDouble()) / num, (j + rand.randomDouble()) / num);
                samples.push_back(sample);
            }
}
