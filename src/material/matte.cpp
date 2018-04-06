#include "matte.h"
#include "core/world.h"

Matte::Matte(vec3 color, float ambient_intensity, float diffuse_intensity) : Material() {
    ambientBRDF = new Lambertian();
    diffuseBRDF = new Lambertian();
    setDiffuseColor(color);
    setAmbientIntensity(ambient_intensity);
    setDiffuseIntensity(diffuse_intensity);
}

vec3 Matte::shade(Shade &shade) {
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
                color += diffuseBRDF->calcBRDF(shade, in, out) * light->incidRadiosity(shade)
                    * float(light->geometryTerm(shade) * nDotIn / light->probDensity(shade));
        }
    }

    return color;
}