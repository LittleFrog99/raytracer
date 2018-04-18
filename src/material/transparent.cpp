#include "transparent.h"
#include "sampler/multijittered.h"
#include "core/world.h"

Transparent::Transparent(vec3 color, float amb_int, float diff_int, float spec_int,
                         float refl_int, float trans_int, float ior)
    : Phong(color, amb_int, diff_int, spec_int)
{
    reflBRDF = new PerfectSpecular(refl_int);
    specBTDF = new PerfectTransmitter(trans_int, ior);
}

vec3 Transparent::shade(Shade &shade) {
    return commonShade(shade, shade.depth + 1);
}

vec3 Transparent::globalShade(Shade &shade) {
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