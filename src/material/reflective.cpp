#include "reflective.h"
#include "core/world.h"
#include "sampler/multijittered.h"
#include "photon/photontracer.h"

Reflective::Reflective(vec3 color, float amb_int, float diff_int,
           float spec_int, float reflec_int)
    : Phong(color, amb_int, diff_int, spec_int)
{
    reflBRDF = new PerfectSpecular();
    reflBRDF->setIntensity(reflec_int);
    reflBRDF->setColor(color);

    float range = 0;
    behavior[pair<float, float>(range, range + diff_int)] = DIFFUSE;
    range += diff_int;
    behavior[pair<float, float>(range, range + spec_int)] = SPECULAR;
    range += spec_int;
    behavior[pair<float, float>(range, range + reflec_int)] = REFLECTION;
}

vec3 Reflective::shade(Shade &shade) {
    return commonShade(shade, shade.depth + 1);
}

vec3 Reflective::pathShade(Shade &shade) {
    return commonShade(shade, shade.depth == 0 ? shade.depth + 2 : shade.depth + 1);
}

vec3 Reflective::commonShade(Shade &shade, int depth) {
    vec3 color = Phong::shade(shade);

    dvec3 in;
    dvec3 out = -shade.ray.direction;
    vec3 brdf = reflBRDF->sampleBRDF(shade, in, out);
    Ray reflRay = Ray(shade.hitPoint, in);

    color += brdf * shade.world.tracerP->traceRay(reflRay, depth);

    return color;
}

void Reflective::photonInteract(Shade &shade, PhotonMap *map, Photon *photon)
{
    PhotonScatter scatter = PhotonTracer::scatterBehavior(behavior);
    dvec3 in = -photon->getDirection(), out;
    vec3 reflectance;

    switch (scatter) {
        case DIFFUSE:
            if (photon->bounce > 0)
                map->store(photon);

            reflectance = float(PI) * diffBRDF->sampleBRDF(shade, out, in);
            break;

        case SPECULAR:
            reflectance = specBRDF->sampleBRDF(shade, out, in);
            break;

        case REFLECTION:
            reflectance = reflBRDF->sampleBRDF(shade, out, in);
            break;

        default:
            return;

    }

    Photon newPhoton;
    newPhoton.position = photon->position;
    newPhoton.setDirection(out);
    newPhoton.power = reflectance * photon->power;
    newPhoton.bounce = photon->bounce + 1;

    PhotonTracer::tracePhoton(map, &newPhoton);
}

vec3 Reflective::photonShade(Shade &shade) {
    vec3 color;

    color = Phong::shade(shade);
    dvec3 in, out;
    color += shade.world.photonMap->estimateIrradiance(shade) 
        * diffBRDF->calcBRDF(shade, in, out) * float(PI);

    out = -shade.ray.direction;
    vec3 brdf = reflBRDF->sampleBRDF(shade, in, out);
    Ray reflRay = Ray(shade.hitPoint, in);
    color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1);
    
    return color;
}

Reflective::~Reflective() {
    Phong::~Phong();
    delete reflBRDF;
}
