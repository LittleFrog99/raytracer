#include "sampler.h"
#include "utilities.h"
#include <algorithm>

void Sampler::setupShuffledIndices() {
    shuffledIndices.reserve(numSamples * numSets);
    vector<int> indices;

    for (int i = 0;i < numSamples; i++)
        indices.push_back(i);

    for (int p = 0; p < numSets; p++) {
        random_shuffle(indices.begin(), indices.end());
        for (int i = 0; i < numSamples; i++)
            shuffledIndices.push_back(indices[i]);
    }
}

dvec2 Sampler::sampleUnitSquare() {
    Random rand;
    if (count % numSamples == 0) // start of a new pixel
        jump = (rand.randomInteger() % numSets) * numSamples;
    return samples[jump + shuffledIndices[jump + count++ % numSamples]];
}
