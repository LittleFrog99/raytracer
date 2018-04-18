#include "matte.h"
#include "core/world.h"

Matte::Matte(vec3 color, float ambient_intensity, float diffuse_intensity) : Material() {
    ambBRDF = new Lambertian();
    diffBRDF = new Lambertian();
    setDiffuseColor(color);
    setAmbientIntensity(ambient_intensity);
    setDiffuseIntensity(diffuse_intensity);
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

vec3 Matte::globalShade(Shade &shade) {
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

Matte::~Matte() {
    delete ambBRDF;
    delete diffBRDF;
}