#include "phong.h"
#include "core/world.h"
#include "sampler/multijittered.h"

Phong::Phong(vec3 color, float amb_int, float diff_int, float spec_int) {
    ambientBRDF = new Lambertian();
    diffuseBRDF = new Lambertian();
    specularBRDF = new Specular();
    setAmbientIntensity(amb_int);
    setDiffuseColor(color);
    setDiffuseIntensity(diff_int);
    setSpecularColor(vec3(1.0));
    setSpecularIntensity(spec_int);
    setSpecularExponent(16.0f);
}

vec3 Phong::shade(Shade &shade) {
    dvec3 out = -shade.ray.direction;
    vec3 color = ambientBRDF->calcReflectance(shade, out) *
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
                color += (diffuseBRDF->calcBRDF(shade, in, out) 
                + specularBRDF->calcBRDF(shade, in, out)) * light->incidRadiosity(shade) 
                * light->geometryTerm(shade) * nDotIn / light->probDensity(shade);
        }
    }

    return color;
}

vec3 Phong::globalShade(Shade &shade) {
    vec3 color = Phong::shade(shade);
    
    dvec3 in, out = -shade.ray.direction;
    float probDensity;

    vec3 brdf = diffuseBRDF->sampleF(shade, in, out, &probDensity); // diffuse layer
    float nDotIn = dot(shade.normal, in);
    Ray reflRay = Ray(shade.hitPoint, in);
    color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1)
             * nDotIn / probDensity;
    
    brdf = specularBRDF->sampleF(shade, in, out, &probDensity); // specular layer
    nDotIn = dot(shade.normal, in);
    reflRay = Ray(shade.hitPoint, in);
    color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1)
             * nDotIn / probDensity;
    
    return color;
}

Phong::~Phong() {
    delete ambientBRDF;
    delete diffuseBRDF;
    delete specularBRDF;
}