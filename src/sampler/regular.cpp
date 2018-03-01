#include "regular.h"
#include "utilities.h"

Regular::Regular(int num) : Sampler(num) {
    generateSamples();
}

void Regular::generateSamples() {
    Random rand;
    int num = int(sqrt(numSamples));

    for (int p = 0; p < numSets; p++)
         for (int i = 0; i < num; i++)
             for (int j = 0; j < num; j++)
                 samples.push_back(dvec2((j + 0.5)/num, (i + 0.5)/num));
}
