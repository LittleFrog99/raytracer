#pragma once

#include "material/phong.h"
#include "bsdf/fresnelreflector.h"
#include "bsdf/fresneltransmitter.h"
#include "bsdf/perfecttransmitter.h"

class Dielectric : public Phong {
public:
    Dielectric(vec3 color, float amb_int, float diff_int, float spec_int, float eta_in, 
               float eta_out, vec3 filter_in = vec3(1.0), vec3 filter_out = vec3(1.0));
    virtual vec3 shade(Shade &shade);
    virtual vec3 globalShade(Shade &shade);

private:
    float etaIn, etaOut;
    vec3 filterIn, filterOut;
    FresnelReflector *fresnelBRDF;
    FresnelTransmitter *fresnelBTDF;

    vec3 commonShade(Shade &shade, int depth);
};