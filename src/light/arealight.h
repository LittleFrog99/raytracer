#pragma once

#include "core/light.h"
#include "core/geometry.h"

class AreaLight : public Light {
public:
    enum Attenuation { LINEAR, SQUARE };

    AreaLight() {}
    AreaLight(Geometry *object_ptr) : objectP(object_ptr) {}
    virtual dvec3 getDirection(Shade &shade);
    virtual vec3 incidRadiosity(Shade &shade);
    virtual bool inShadow(Ray &shadow_ray, Shade &shade);
    virtual float probDensity(Shade &shade);
    virtual float geometryTerm(Shade &shade);
    virtual ~AreaLight() {}

    inline void setAttenuationType(Attenuation type) {
        atten = type;
    }

private:
    Geometry *objectP = nullptr;
    dvec3 samplePt;
    dvec3 normal;
    dvec3 out;
    Attenuation atten = LINEAR;
};