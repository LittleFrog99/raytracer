#include "reflective.h"
#include "core/world.h"
#include "sampler/multijittered.h"

Reflective::Reflective(vec3 color, float amb_int, float diff_int,
           float spec_int, float reflec_int)
    : Phong(color, amb_int, diff_int, spec_int)
{
    reflBRDF = new PerfectSpecular();
    reflBRDF->setIntensity(reflec_int);
    reflBRDF->setColor(color);
}

vec3 Reflective::shade(Shade &shade) {
    vec3 color = Phong::shade(shade);
    dvec3 out = -shade.ray.direction;
    dvec3 in;
    vec3 reflFactor = reflBRDF->sampleBRDF(shade, in, out);

    Ray refRay = Ray(shade.hitPoint, in);
    color += reflFactor * shade.world.tracerP->traceRay(refRay, shade.depth + 1);

    return color;
}

vec3 Reflective::globalShade(Shade &shade) {
    vec3 color = Phong::shade(shade);
    
    dvec3 in;
    dvec3 out = -shade.ray.direction;
    float pdf;
    vec3 brdf = reflBRDF->sampleBRDF(shade, in, out, &pdf);
    Ray reflRay = Ray(shade.hitPoint, in);

    int newDepth = shade.depth == 0 ? shade.depth + 2 : shade.depth + 1;
    color += brdf * shade.world.tracerP->traceRay(reflRay, newDepth) 
             * float(dot(shade.normal, in)) / pdf;
    
    return color;
}

Reflective::~Reflective() {
    Phong::~Phong();
    delete reflBRDF;
}
