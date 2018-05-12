#pragma once

#include "utilities.h"
#include "core/shade.h"
#include "core/sampler.h"

struct BSSRDFTable {
    vector<float> albedo, effAlbedo, radius, profile;

    BSSRDFTable() {}
    BSSRDFTable(int num_albedo_samples, int num_radius_samples);

    inline float evaluateProfile(int albedoIndex, int radiusIndex) {
        return profile[albedoIndex * radius.size() + radiusIndex];
    }
};

class BSSRDF {
public:
    virtual vec3 calcS(Shade &po, Shade &pi, dvec3 &wi) = 0;
    virtual vec3 sampleS(Shade &po, double u1, const dvec2 &u2, Shade &pi, double *pdf) = 0;

protected:
    float fresnelMoment1(float eta);
    float fresnelMoment2(float eta);
};