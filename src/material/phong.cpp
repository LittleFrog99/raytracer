#include "phong.h"
#include "core/world.h"
#include "sampler/multijittered.h"

Phong::Phong(vec3 color, float amb_int, float diff_int, float spec_int) {
    ambBRDF = new Lambertian();
    diffBRDF = new Lambertian();
    specBRDF = new Specular();
    setAmbientIntensity(amb_int);
    setDiffuseColor(color);
    setDiffuseIntensity(diff_int);
    setSpecularColor(vec3(1.0));
    setSpecularIntensity(spec_int);
    setSpecularExponent(DEFAULT_SPECULAR_EXPONENT);
}

vec3 Phong::shade(Shade &shade) {
    dvec3 out = -shade.ray.direction;
    vec3 color = ambBRDF->calcReflectance(shade, out) *
                 shade.world.ambientP->incidRadiosity(shade);

    for (int i = 0; i < shade.world.lights.size(); i++) {
        Light *light = shade.world.lights[i];
        dvec3 in = light->getDirection(shade);
        float nDotIn = dot(shade.normal, in);

        if (nDotIn > 0.0) {
            bool inShadow = false;
            if (shade.world.lights[i]->castShadow()) {
                Ray shadowRay(shade.hitPoint, in);
                inShadow = light->inShadow(shadowRay, shade);
            }

            if (!inShadow)
                color += (diffBRDF->calcBRDF(shade, in, out) 
                + specBRDF->calcBRDF(shade, in, out)) * light->incidRadiosity(shade) 
                * light->geoTerm(shade) * nDotIn / light->probDenFunc(shade);
        }
    }

    return color;
}

vec3 Phong::globalShade(Shade &shade) {
    vec3 color = Phong::shade(shade);
    
    dvec3 in, out = -shade.ray.direction;
    float pdf;

    vec3 brdf = diffBRDF->sampleBRDF(shade, in, out, &pdf); // diffuse layer
    float nDotIn = dot(shade.normal, in);
    Ray reflRay = Ray(shade.hitPoint, in);
    color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1)
             * nDotIn / pdf;
    
    brdf = specBRDF->sampleBRDF(shade, in, out, &pdf); // specular layer
    nDotIn = dot(shade.normal, in);
    reflRay = Ray(shade.hitPoint, in);
    color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1)
             * nDotIn / pdf;
    
    return color;
}

Phong::~Phong() {
    delete ambBRDF;
    delete diffBRDF;
    delete specBRDF;
}