#pragma once

#include "core/material.h"

class Emissive : public Material {
public:
    Emissive(vec3 color, float intensity) : color(color), intensity(intensity) {}
    virtual vec3 getEmissiveLight(Shade &shade);
    virtual vec3 shade(Shade &shade);
    virtual vec3 pathShade(Shade &shade);
    virtual vec3 photonShade(Shade &shade);
    virtual void photonInteract(Shade &shade, PhotonMap *map, Photon *photon);

    inline void setIntensity(float intensity) {
        this->intensity = intensity;
    }
    inline void setColor(vec3 color) {
        this->color = color;
    }

private:
    vec3 color;
    float intensity;
};