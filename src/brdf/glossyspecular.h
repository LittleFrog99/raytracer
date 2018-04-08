#pragma once

#include "brdf/specular.h"

class GlossySpecular : public Specular {
public:
    GlossySpecular() {}
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *prob_den = nullptr);
};