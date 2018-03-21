#pragma once

#include "utilities.h"

class Sampler {
public:
    Sampler (int nspl, int nset) : numSamples(nspl), numSets(nset) {}
    void mapSamplesToUnitDisk();
    void mapSamplesToHemisphere(double exponent = 1);
    dvec2 sampleUnitSquare();
    dvec2 sampleUnitDisk();
    dvec3 sampleUnitHemisphere();
    inline int getNumSamples() { return numSamples; }
    virtual ~Sampler() {}

protected:
    int numSamples;
    int numSets;
    vector<dvec2> samples;
    vector<dvec2> diskSamples;
    vector<dvec3> hemisphereSamples;
    vector<int> shuffledIndices;
    int count = 0;
    int jump;

    virtual void generateSamples() = 0;
    void setupShuffledIndices();
};
