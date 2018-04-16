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

    dvec3 out = -shade.ray.direction;
    dvec3 in;
    vec3 brdf = reflBRDF->sampleBRDF(shade, in, out);
    Ray reflRay = Ray(shade.hitPoint, in);
    vec3 reflection = shade.world.tracerP->traceRay(reflRay, shade.depth + 1);

    if (specBTDF->isTIR(shade)) 
        color += reflection;
    else {
        color += brdf * reflection * float(fabs(dot(shade.normal, in)));

        dvec3 trans;
        vec3 btdf = specBTDF->sampleBTDF(shade, trans, out);
        Ray transRay = Ray(shade.hitPoint, trans);
        
        color += btdf * shade.world.tracerP->traceRay(transRay, shade.depth + 1) 
                 * float(fabs(dot(shade.normal, trans)));
    }

    return color;
}

vec3 Transparent::globalShade(Shade &shade) {
    vec3 color = Phong::shade(shade);

    dvec3 out = -shade.ray.direction;
    dvec3 in, trans;
    vec3 brdf = reflBRDF->sampleBRDF(shade, in, out);
    Ray reflRay = Ray(shade.hitPoint, in);
    
    int newDepth = shade.depth == 0 ? shade.depth + 2 : shade.depth + 1;
    vec3 reflection = shade.world.tracerP->traceRay(reflRay, newDepth);

    if (specBTDF->isTIR(shade)) 
        color += reflection;
    else {
        color += brdf * reflection * float(fabs(dot(shade.normal, in)));

        vec3 btdf = specBTDF->sampleBTDF(shade, trans, out);
        Ray transRay = Ray(shade.hitPoint, trans);
        vec3 transmission = shade.world.tracerP->traceRay(transRay, newDepth);
        
        color += btdf * transmission * float(fabs(dot(shade.normal, trans)));
    }
    
    return color;
}