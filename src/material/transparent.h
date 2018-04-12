#pragma once

#include "material/phong.h"
#include "bsdf/perfectspecular.h"
#include "bsdf/perfecttransmitter.h"

class Transparent : public Phong {
public:
    Transparent() {}
    Transparent(vec3 color, float amb_int, float diff_int, float spec_int, 
                float reflec_int, float trans_int, float ior);
    virtual vec3 shade(Shade &shade);

private:
    PerfectSpecular *reflBRDF;
    PerfectTransmitter *specBTDF;
};