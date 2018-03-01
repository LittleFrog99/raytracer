#include "viewplane.h"
#include "sampler/regular.h"
#include "sampler/jittered.h"
#include "sampler/multijittered.h"

void ViewPlane::setSampler(Sampler *sp) {
    if (samplerP) {
        delete samplerP;
        samplerP = nullptr;
    }

    numSamples = sp->getNumSamples();
    samplerP = sp;
}

void ViewPlane::setSamples(const int num_sampler, const int num_sets) {
    if (samplerP) {
        delete samplerP;
        samplerP = nullptr;
    }

    if (numSamples > 1)
        samplerP = new MultiJittered(num_sampler, num_sets);
    else
        samplerP = new Regular(1);
}
