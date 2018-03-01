#include "viewplane.h"
#include "sampler/jittered.h"

void ViewPlane::setSampler(Sampler *sp) {
    if (samplerP) {
        delete samplerP;
        samplerP = nullptr;
    }

    numSamples = sp->getNumSamples();
    samplerP = sp;
}

void ViewPlane::setSamples(const int n) {
    numSamples = n;
    if (samplerP) {
        delete samplerP;
        samplerP = nullptr;
    }

    if (numSamples > 1)
        samplerP = new Jittered(n);
}
