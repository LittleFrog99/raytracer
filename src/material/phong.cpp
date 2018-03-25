#include "phong.h"
#include "core/world.h"
#include "sampler/multijittered.h"

Phong::Phong(vec3 color, float amb_int, float diff_int, float spec_int) {
    ambientBRDF = new Lambertian();
    diffuseBRDF = new Lambertian();
    specularBRDF = new GlossySpecular();
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
        dvec3 in = shade.world.lights[i]->getDirection(shade);
        float nDotIn = dot(shade.normal, in);
        
        if (nDotIn > 0.0) {
            bool inShadow = false;
            if (shade.world.lights[i]->castShadow()) {
                Ray shadowRay(shade.hitPoint, in);
                inShadow = shade.world.lights[i]->inShadow(shadowRay, shade);
            }

            if (!inShadow)
                color += (diffuseBRDF->calcBRDF(shade, in, out) +
                    specularBRDF->calcBRDF(shade, in, out)) *
                    shade.world.lights[i]->incidRadiosity(shade) * nDotIn;
        }        
    }

    return color;
}

vec3 Phong::areaLightShade(Shade &shade) {
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