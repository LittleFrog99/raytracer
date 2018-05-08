#include "transparent.h"
#include "sampler/multijittered.h"
#include "core/world.h"
#include "photon/photontracer.h"

Transparent::Transparent(vec3 color, float amb_int, float diff_int, float spec_int,
                         float refl_int, float trans_int, float ior)
    : Phong(color, amb_int, diff_int, spec_int)
{
    reflBRDF = new PerfectSpecular(refl_int);
    specBTDF = new PerfectTransmitter(trans_int, ior);

    float range = 0;
    behavior[pair<float, float>(range, range + diff_int + EPSILON)] = DIFFUSE;
    range += diff_int + EPSILON;
    behavior[pair<float, float>(range, range + refl_int + EPSILON)] = REFLECTION;
    range += refl_int + EPSILON;
    behavior[pair<float, float>(range, range + trans_int)] = TRANSMISSION;
}

vec3 Transparent::shade(Shade &shade) {
    return commonShade(shade, shade.depth + 1);
}

vec3 Transparent::pathShade(Shade &shade) {
    return commonShade(shade, shade.depth == 0 ? shade.depth + 2 : shade.depth + 1);
}

vec3 Transparent::commonShade(Shade &shade, int depth) {
    vec3 color = Phong::shade(shade);

    dvec3 out = -shade.ray.direction;
    dvec3 in;
    vec3 brdf = reflBRDF->sampleBRDF(shade, in, out);
    Ray reflRay = Ray(shade.hitPoint, in);
    vec3 reflection = shade.world.tracerP->traceRay(reflRay, depth);

    if (specBTDF->isTIR(shade)) 
        color += reflection;
    else {
        color += brdf * reflection * float(fabs(dot(shade.normal, in)));

        dvec3 trans;
        vec3 btdf = specBTDF->sampleBTDF(shade, trans, out);
        Ray transRay = Ray(shade.hitPoint, trans);
        
        color += btdf * shade.world.tracerP->traceRay(transRay, depth);
    }

    return color;
}

void Transparent::photonInteract(Shade &shade, PhotonMap *map, Photon *photon)
{
    PhotonScatter scatter = PhotonTracer::scatterBehavior(behavior);
    dvec3 in = -photon->getDirection(), out;
    vec3 reflectance;
    Photon newPhoton;

    switch (scatter) {
        case DIFFUSE:
            if (photon->totalBounce() > 0) {
                if (photon->isCaustic())
                    map->storeCaustic(photon);
                else
                    map->storeGlobal(photon);
            }

            reflectance = float(PI) * diffBRDF->sampleBRDF(shade, out, in);
            newPhoton.diffBounce = photon->diffBounce + 1;
            break;

        case REFLECTION: case TRANSMISSION:
            if (specBTDF->isTIR(shade)) {
                reflBRDF->sampleBRDF(shade, out, in);
                reflectance = vec3(1.0);
                break;
            }
            if (scatter == REFLECTION) 
                reflectance = reflBRDF->sampleBRDF(shade, out, in);
            else
                reflectance = specBTDF->sampleBTDF(shade, out, in);
            newPhoton.specBounce = photon->specBounce + 1;
            break;

        default:
            return;

    }

    newPhoton.position = photon->position;
    newPhoton.setDirection(out);
    newPhoton.power = reflectance * photon->power;

    PhotonTracer::tracePhoton(map, &newPhoton);
}

vec3 Transparent::photonShade(Shade &shade) {
    vec3 color;
    
    dvec3 in, out = -shade.ray.direction;
    /* color += shade.world.photonMap->estimateIrradiance(shade) 
        * diffBRDF->calcBRDF(shade, in, out) * float(PI);*/

    vec3 brdf = reflBRDF->sampleBRDF(shade, in, out);
    Ray reflRay = Ray(shade.hitPoint, in);
    vec3 reflection = shade.world.tracerP->traceRay(reflRay, shade.depth + 1);

    if (specBTDF->isTIR(shade))
        color += reflection;
    else {
        color += brdf * reflection * float(fabs(dot(shade.normal, in)));

        dvec3 trans;
        vec3 btdf = specBTDF->sampleBTDF(shade, trans, out);
        Ray transRay = Ray(shade.hitPoint, trans);

        color += btdf * shade.world.tracerP->traceRay(transRay, shade.depth + 1);
    }

    return color;
}
