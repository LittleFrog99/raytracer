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
    vec3 color = Phong::shade(shade);

    dvec3 out = -shade.direction;
    dvec3 in;
    vec3 brdf = reflBRDF->sampleBRDF(shade, in, out);
    Ray reflRay = Ray(shade.hitPoint, in);

    if (specBTDF->isTIR(shade)) 
        color += shade.world.tracerP->traceRay(reflRay, shade.depth + 1);
    else {
        dvec3 trans;
        vec3 btdf = specBTDF->sampleBTDF(shade, trans, out);
        Ray transRay = Ray(shade.hitPoint, trans);
        color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1) 
                 * float(fabs(dot(shade.normal, in)));
        color += btdf * shade.world.tracerP->traceRay(transRay, shade.depth + 1) 
                 * float(fabs(dot(shade.normal, trans)));
    }

    return color;
}
