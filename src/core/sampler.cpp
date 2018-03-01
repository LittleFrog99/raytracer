#include "sampler.h"
#include "utilities.h"

dvec2 Sampler::sampleUnitSquare() {
    Random rand;
    if (count % numSamples == 0) // start of a new pixel
        jump = (rand.randomInteger() % numSets) * numSamples;
    return samples[jump + count++ % numSamples];
}
