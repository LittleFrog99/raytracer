#include "matte.h"
#include "core/world.h"
#include "photon/photontracer.h"
#include "debug.h"

Matte::Matte(vec3 color, float ambient_intensity, float diffuse_intensity) : Material() {
    ambBRDF = new Lambertian(ambient_intensity, color);
    diffBRDF = new Lambertian(diffuse_intensity, color);
}

vec3 Matte::shade(Shade &shade) {
    dvec3 out = -shade.ray.direction;
    vec3 color = ambBRDF->calcReflectance(shade, out) * 
        shade.world.ambientP->incidRadiance(shade);

    for (Light *lightP : shade.world.lights) {
        dvec3 in = lightP->calcDirection(shade);
        float nDotIn = dot(shade.normal, in);

        if (nDotIn > 0.0) {
            bool inShadow = false;
            if (lightP->castShadow()) {
                Ray shadowRay(shade.hitPoint, in);
                inShadow = lightP->inShadow(shadowRay, shade);
            }

            if (!inShadow)
                color += diffBRDF->calcBRDF(shade, in, out) * lightP->incidRadiance(shade)
                    * float(lightP->geoTerm(shade) * nDotIn / lightP->calcPdf(shade));
        }
    }

    return color;
}

vec3 Matte::pathShade(Shade &shade) {
    vec3 color = Matte::shade(shade);
    
    dvec3 in, out = -shade.ray.direction;
    float pdf;

    vec3 brdf = diffBRDF->sampleBRDF(shade, in, out, &pdf);
    float nDotIn = dot(shade.normal, in);
    Ray reflRay = Ray(shade.hitPoint, in);
    color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1)
             * nDotIn / pdf;
    
    return color;
}

void Matte::photonInteract(Shade &shade, PhotonMap *map, Photon *photon) {
    if (photon == nullptr) return;
    if (roulette() < diffBRDF->intensity) {
        if (photon->totalBounce() > 0) { // exclude direct illumination
            if (photon->isCaustic()) 
                map->storeCaustic(photon);
            else
                map->storeGlobal(photon);
        }
        
        dvec3 in = -photon->getDirection(), out;
        vec3 brdf = diffBRDF->sampleBRDF(shade, out, in);

        Photon newPhoton;
        newPhoton.position = photon->position;
        newPhoton.setDirection(out);
        newPhoton.power = float(PI) * brdf * photon->power;
        newPhoton.diffBounce = photon->diffBounce + 1;

        PhotonTracer::tracePhoton(map, &newPhoton);
    }
}

vec3 Matte::photonShade(Shade &shade) {
    vec3 color;
    color = Matte::shade(shade);
    dvec3 in, out;
    color += shade.world.photonMap->estimateIrradiance(shade) * diffBRDF->calcBRDF(shade, in, out);
    return color;
}

Matte::~Matte() {
    delete ambBRDF;
    delete diffBRDF;
}