#pragma once

#include "core/bssrdf.h"

class Separable : public BSSRDF {
public:
    Separable(Material *mat_ptr, float eta);
    virtual vec3 calcS(Shade &po, Shade &pi, dvec3 &wi);
    virtual vec3 sampleS(Shade &po, double u1, const dvec2 &u2, Shade &pi, float *pdf);
    virtual float sampleSw(Shade &pi, dvec3 &wi, float *pdf);
    virtual ~Separable();

protected:
    Material *materialP;
    float eta;
    Sampler *smplrP = nullptr;
    
    float calcSw(Shade &pi, dvec3 &wi); // direction term
    vec3 calcSp(Shade &po, Shade &pi); // spatial term
    float fresnelReflFactor(float cos_theta);
    vec3 sampleSp(Shade &po, double u1, const dvec2 &u2, Shade &pi, float *pdf);

    virtual vec3 calcSr(float distance) = 0; // distance term
    virtual float sampleSr(int channel, double u) = 0;
    virtual float pdfSr(int channel, double radius) = 0;
};