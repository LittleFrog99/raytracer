#pragma once

#include "core/light.h"
#include "core/geometry.h"

class AreaLight : public Light {
public:
    enum Attenuation { LINEAR, SQUARE };

    AreaLight(Geometry *object_ptr);
    virtual dvec3 calcDirection(Shade &shade);
    virtual vec3 incidRadiance(Shade &shade);
    virtual bool inShadow(Ray &shadow_ray, Shade &shade);
    virtual float probDenFunc(Shade &shade);
    virtual float geoTerm(Shade &shade);
    virtual void emitPhotons(PhotonMap *map, int num);

private:
    Geometry *objectP = nullptr;
    dvec3 samplePt;
    dvec3 normal;
    dvec3 out;
};