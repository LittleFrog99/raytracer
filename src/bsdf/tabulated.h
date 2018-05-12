#pragma once

#include "separable.h"

class Tabulated : public Separable {
public:
    Tabulated(float eta, vec3 absorp, vec3 scatter, BSSRDFTable *table);

private:
    BSSRDFTable *table;
    vec3 extinction; // sigma_t
    vec3 albedo; // rho

    virtual vec3 calcSr(float distance);
};