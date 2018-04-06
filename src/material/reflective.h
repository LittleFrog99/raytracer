#pragma once

#include "material/phong.h"
#include "brdf/perfectspecular.h"

class Reflective : public Phong {
public:
    Reflective() {}
    Reflective(vec3 color = vec3(), float amb_int = 0.0, float diff_int = 0.0, 
               float spec_int = 0.0, float reflec_int = 0.0);
    virtual vec3 shade(Shade &shade);

    inline void setReflectiveIntensity(float value) {
        reflectiveBRDF->setIntensity(value);
    }

    inline void setReflectiveColor(vec3 color) {
        reflectiveBRDF->setColor(color);
    }

    inline void setReflectiveSampler(Sampler *sampler_ptr) {
        reflectiveBRDF->setSampler(sampler_ptr, 1.0);
    }

private:
    PerfectSpecular *reflectiveBRDF;
};