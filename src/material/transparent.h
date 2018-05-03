#pragma once

#include "material/phong.h"
#include "bsdf/perfectspecular.h"
#include "bsdf/perfecttransmitter.h"

class Transparent : public Phong {
public:
    Transparent(vec3 color, float amb_int, float diff_int, float spec_int, 
                float reflec_int, float trans_int, float ior);
    virtual vec3 shade(Shade &shade);
    virtual vec3 pathShade(Shade &shade);
    virtual vec3 photonShade(Shade &shade);
    virtual void photonInteract(Shade &shade, PhotonMap *map, Photon *photon);

private:
    PerfectSpecular *reflBRDF;
    PerfectTransmitter *specBTDF;
    map<pair<float, float>, PhotonScatter> behavior;

    vec3 commonShade(Shade &shade, int depth);
};