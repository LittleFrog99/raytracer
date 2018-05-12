#pragma once

#include "separable.h"

class Tabulated : public Separable {
public:
    Tabulated(Material *mat_ptr, float eta, vec3 absorp, vec3 scatter, BSSRDFTable *table);

private:
    BSSRDFTable *table;
    vec3 extinc; // sigma_t
    vec3 albedo; // rho

    virtual vec3 calcSr(float distance);
    virtual float sampleSr(int channel, double u);
    virtual float pdfSr(int channel, double radius);
};