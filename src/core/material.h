#pragma once

#include "utilities.h"
#include "photon/photonmap.h"

struct Shade;

enum PhotonScatter {
    DIFFUSE, 
    SPECULAR,
    REFLECTION,
    TRANSMISSION, 
    ABSORPTION
};

class Material {
public:
    virtual vec3 getEmissiveLight(Shade &shade) { return Color::BLACK; }
    virtual vec3 shade(Shade &shade) { return Color::BLACK; }
    virtual vec3 pathShade(Shade &sr) { return shade(sr); }
    virtual vec3 photonShade(Shade &shade) { return Color::BLACK; }
    virtual void photonInteract(Shade &shade, PhotonMap *map, Photon *photon) { }
    virtual ~Material() {}
};
