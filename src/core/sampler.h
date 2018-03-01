#pragma once

#include <glm/glm.hpp>
#include <vector>
using namespace glm;
using namespace std;

class Sampler {
public:
    Sampler (int num) : numSamples(num) {}
    void setupShuffledIndices();
    void shuffleSamples();
    dvec2 sampleUnitSquare();
    inline int getNumSamples() { return numSamples; }
    virtual ~Sampler() {}

protected:
    int numSamples;
    int numSets;
    vector<dvec2> samples;
    vector<int> shuffledIndices;
    unsigned long count;
    int jump;

    virtual void generateSamples() = 0;
};
