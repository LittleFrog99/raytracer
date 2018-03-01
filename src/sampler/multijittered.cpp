#include "multijittered.h"
#include "utilities.h"

MultiJittered::MultiJittered(int num) : Sampler(num) {
    generateSamples();
}

void MultiJittered::generateSamples() {
    Random rand;
    int num = int(sqrt(numSamples)); // numSamples must be perfect square
    double subcell_width = 1.0 / float(numSamples);

    samples.reserve(numSamples * numSets);

    for (int p = 0; p < numSets; p++) // initial patterns
        for (int i = 0; i < num; i++)
            for (int j = 0; j < num; j++) {
                double x = (i * num + j) * subcell_width + rand.randomDouble() * subcell_width;
                double y = (j * num + i) * subcell_width + rand.randomDouble() * subcell_width;
                samples[p * numSamples + i * num + j] = dvec2(x, y);
            }

    for (int p = 0; p < numSets; p++) // shuffle x coordinates
        for (int i = 0; i < num; i++)
            for (int j = 0; j < num; j++) {
                int k = rand.randomInteger(j, num - 1);
                swap(samples[p * numSamples + i * num + j].x, samples[p * numSamples + i * num + k].x);
            }

    for (int p = 0; p < numSets; p++) // shuffle y coordinates
        for (int i = 0; i < num; i++)
            for (int j = 0; j < num; j++) {
                int k = rand.randomInteger(j, num - 1);
                swap(samples[p * numSamples + j * num + i].y, samples[p * numSamples + k * num + i].y);
            }
}
