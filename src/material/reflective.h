#pragma once

#include "material/phong.h"
#include "bsdf/perfectspecular.h"

class Reflective : public Phong {
public:
    Reflective(vec3 color, float amb_int, float diff_int, float spec_int, float refl_int);
    virtual vec3 shade(Shade &shade);
    virtual vec3 pathShade(Shade &shade);
    virtual ~Reflective();

    inline void setReflectiveIntensity(float value) {
        reflBRDF->setIntensity(value);
    }

    inline void setReflectiveColor(vec3 color) {
        reflBRDF->setColor(color);
    }

    inline void setReflectiveSampler(Sampler *sampler_ptr) {
        reflBRDF->setSampler(sampler_ptr);
    }

protected:
    PerfectSpecular *reflBRDF;

private:
    vec3 commonShade(Shade &shade, int depth);
};