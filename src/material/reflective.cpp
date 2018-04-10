#include "reflective.h"
#include "core/world.h"

Reflective::Reflective(vec3 color, float amb_int, float diff_int,
           float spec_int, float reflec_int)
    : Phong(color, amb_int, diff_int, spec_int)
{
    reflectiveBRDF = new PerfectSpecular();
    reflectiveBRDF->setIntensity(reflec_int);
    reflectiveBRDF->setColor(color);
}

vec3 Reflective::shade(Shade &shade) {
    vec3 color = Phong::shade(shade);
    dvec3 out = -shade.ray.direction;
    dvec3 in;
    vec3 reflFactor = reflectiveBRDF->sampleF(shade, in, out);

    Ray refRay = Ray(shade.hitPoint, in);
    color += reflFactor * shade.world.tracerP->traceRay(refRay, shade.depth + 1);

    return color;
}

vec3 Reflective::globalShade(Shade &shade) {
    vec3 color;
    if (shade.depth == 0)
        color = Phong::shade(shade);
    
    dvec3 in, out = -shade.ray.direction;
    float probDensity;
    vec3 brdf = reflectiveBRDF->sampleF(shade, in, out, &probDensity);
    Ray reflRay = Ray(shade.hitPoint, in);

    if (shade.depth == 0)
        color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 2)
                 * float(dot(shade.normal, in)) / probDensity;
    else
        color += color + brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1) 
                 * float(dot(shade.normal, in)) / probDensity;
    
    return color;
}

Reflective::~Reflective() {
    Phong::~Phong();
    delete reflectiveBRDF;
}
