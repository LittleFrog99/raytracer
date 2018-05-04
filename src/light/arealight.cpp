#include "arealight.h"
#include "core/world.h"
#include "sampler/multijittered.h"
#include "photon/photontracer.h"
#include "debug.h"

AreaLight::AreaLight(Geometry *object_ptr) : objectP(object_ptr)
{
    samplerP = new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS);
    samplerP->mapSamplesToHemisphere();
}

dvec3 AreaLight::calcDirection(Shade &shade) {
    samplePt = objectP->sample();
    normal = objectP->getNormal(samplePt);
    out = normalize(samplePt - shade.hitPoint);
    return out;
}

vec3 AreaLight::incidRadiance(Shade &shade) {
    float nDotD = dot(-normal, out);
    return (nDotD > 0.0) ? objectP->getMaterial()->getEmissiveLight(shade) : Color::BLACK;
}

bool AreaLight::inShadow(Ray &ray, Shade &shade) {
    double t;
    double ts = dot(samplePt - ray.origin, ray.direction);

    for (int i = 0; i < shade.world.objects.size(); i++)
        if (shade.world.objects[i]->shadowIntersect(ray, t) && (t < ts))
            return true;
    
    return false;
}

float AreaLight::probDenFunc(Shade &shade) {
    return objectP->probDenFunc(shade);
}

float AreaLight::geoTerm(Shade &shade) {
    float nDotD = dot(-normal, out);
    float dSquared = Math::distSq(samplePt, shade.hitPoint);
    return nDotD / dSquared;
}

void AreaLight::emitPhotons(PhotonMap *map, int num) {
    int count = 0;
    while (count < num) {
        dvec3 position = objectP->sample();
        dvec3 normal = objectP->getNormal(position);

        dvec3 w = normal;
        dvec3 v = normalize(cross(UP_VECTOR, w));
        dvec3 u = cross(v, w);
        dvec3 samplePt = samplerP->sampleUnitHemisphere();
        dvec3 direction = normalize(samplePt.x * u + samplePt.y * v + samplePt.z * w);

        Shade shade(*map->world);
        vec3 irradiance = objectP->getMaterial()->getEmissiveLight(shade) * float(dot(normal, direction));

        auto photon = new Photon;
        photon->position = position;
        photon->setDirection(direction);
        photon->power = irradiance;
        photon->bounce = 0;
        
        PhotonTracer::tracePhoton(map, photon);
        count++;
    }
    
    map->scalePhotonPower(1.0 / num / PI);
}