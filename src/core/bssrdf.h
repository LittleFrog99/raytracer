#pragma once

#include "utilities.h"
#include "core/shade.h"
#include "core/sampler.h"

struct BSSRDFTable {
    int nAlbedo, nRadius;
    vector<float> albedo, effAlbedo, radius, profile, profileCDF;

    BSSRDFTable() {}
    BSSRDFTable(int num_albedo_samples, int num_radius_samples);

    inline float evaluateProfile(int albedoIndex, int radiusIndex) {
        return profile[albedoIndex * radius.size() + radiusIndex];
    }
};

class BSSRDF {
public:
    virtual vec3 calcS(Shade &po, Shade &pi, dvec3 &wi) = 0;
    virtual vec3 sampleS(Shade &po, double u1, const dvec2 &u2, Shade &pi, float *pdf) = 0;
    virtual ~BSSRDF() {}

protected:
    static float fresnelReflFactor(float cos_theta, float eta);
    static float fresnelMoment1(float eta);
    static float fresnelMoment2(float eta);
    static float phaseHG(float cosTheta, float g);
    static void tangentSpace(const dvec3 &w, dvec3 &u, dvec3 &v);
    static float beamDiffusionMS(float scatter, float absorp, float g, float eta, float r);
    static float beamDiffusionSS(float scatter, float absorp, float g, float eta, float r);
    static void calcBeamDiffusion(float g, float eta, BSSRDFTable *table);
};