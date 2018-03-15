#include "phong.h"
#include "core/world.h"
#include "sampler/multijittered.h"

Phong::Phong(vec3 color, float amb_int, float diff_int, float spec_int) {
    ambientBRDF = new Lambertian(new MultiJittered(25, 2), amb_int, color);
    diffuseBRDF = new Lambertian(new MultiJittered(25, 2), diff_int, color);
    specularBRDF = new GlossySpecular(new MultiJittered(25, 2), spec_int, vec3(1.0f), 8.0);
}

vec3 Phong::shade(Shade &shade) {
    dvec3 out = -shade.ray.direction;
    vec3 color = ambientBRDF->calcReflectance(shade, out) * 
        shade.world.ambientP->incidRadiosity(shade);
    
    for (int i = 0; i < shade.world.lights.size(); i++) {
        dvec3 in = shade.world.lights[i]->getDirection(shade);
        float nDotWi = dot(shade.normal, in);
        if (nDotWi > 0.0)
            color += (diffuseBRDF->calcBRDF(shade, in, out) +
                specularBRDF->calcBRDF(shade, in, out)) * 
                shade.world.lights[i]->incidRadiosity(shade) * nDotWi;
    }

    return color;
}