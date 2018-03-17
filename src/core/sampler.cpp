#include "sampler.h"
#include "utilities.h"

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

void Sampler::mapSamplesToUnitDisk() {
    int size = numSamples * numSets;
    double radius, phi;
    diskSamples.reserve(size);

    for (int i = 0; i < size; i++) {
        dvec2 sp(2.0 * samples[i].x - 1.0, 2.0 * samples[i].y - 1.0);
        if (sp.x > -sp.y) {
            if (sp.x > sp.y) {
                radius = sp.x;
                phi = sp.y / sp.x;
            }
            else {
                radius = sp.y;
                phi = 2 - sp.x / sp.y;
            }
        }
        else {
            if (sp.x < sp.y) {
                radius = -sp.x;
                phi = 4 + sp.y / sp.x;
            }
            else {
                radius = -sp.y;
                if (sp.y != 0.0) // avoid division by zero at origin (NaN)
                    phi = 6 - sp.x / sp.y;
                else
                    phi = 0;
            }
        }
        phi *= PI / 4.0;
        diskSamples[i] = dvec2(radius * cos(phi), radius * sin(phi));
    }
}

dvec2 Sampler::sampleUnitDisk() {
    Random rand;
    if (count % numSamples == 0) // start of a new pixel
        jump = (rand.randomInteger() % numSets) * numSamples;
    return diskSamples[jump + shuffledIndices[jump + count++ % numSamples]];
}

void Sampler::mapSamplesToHemisphere(double exponent) {
    int size = numSamples * numSets;
    hemisphereSamples.reserve(size);

    for (int i = 0; i < size; i++) {
        double phi = 2.0 * PI * samples[i].x;
        double cos_phi = cos(phi), sin_phi = sin(phi);
        double cos_theta = pow((1.0 - samples[i].y), 1.0 / (exponent + 1.0));
        double sin_theta = sqrt(1 - cos_theta * cos_theta);

        dvec3 point(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
        hemisphereSamples.push_back(point);
    }
}

dvec3 Sampler::sampleUnitHemisphere() {
    Random rand;
    if (count % numSamples == 0) // start of a new pixel
        jump = (rand.randomInteger() % numSets) * numSamples;
    return hemisphereSamples[jump + shuffledIndices[jump + count++ % numSamples]];
}
