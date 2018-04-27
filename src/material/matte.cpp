#include "matte.h"
#include "core/world.h"
#include "photon/photontracer.h"
#include "debug.h"

static int lcg() {
    static int x = (unsigned) time(nullptr);
    static const int a = pow(7, 5);
    static const int c = 1;
    static const int m = RAND_MAX;
    x = (x * a + c) % m;
    return x;
}

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
                    * float(lightP->geoTerm(shade) * nDotIn / lightP->probDenFunc(shade));
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
    if (float(lcg()) / RAND_MAX < diffBRDF->intensity) {
        map->addPhoton(photon->position, photon->getDirection(), photon->power);
        // photon->output();
        dvec3 in = -photon->getDirection(), out;
        vec3 brdf = diffBRDF->sampleBRDF(shade, out, in);
        auto newPhoton = new Photon(photon->position, out, float(PI) * brdf * photon->power, photon->bounce + 1);
        PhotonTracer::tracePhoton(map, newPhoton);
    }
    delete photon;
}

vec3 Matte::photonShade(Shade &shade) {
    dvec3 in, out = -shade.ray.direction;
    vec3 color = ambBRDF->calcReflectance(shade, out) *
                 shade.world.ambientP->incidRadiance(shade);
    
    color += diffBRDF->calcBRDF(shade, in, out) * shade.world.photonMap->estimateIrradiance(shade.hitPoint, shade.normal);

    return color;
}

Matte::~Matte() {
    delete ambBRDF;
    delete diffBRDF;
}