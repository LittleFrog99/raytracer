#include "glossyreflector.h"
#include "sampler/multijittered.h"
#include "core/world.h"

GlossyReflector::GlossyReflector(vec3 color, float amb_int, float diff_int, 
    float spec_int, float refl_int) : Phong(color, amb_int, diff_int, spec_int) 
{
    glossySpecularBRDF = new Specular();
    setGlossyReflectionIntensity(refl_int);
    setGlossyReflectionColor(color);
    setGlossyReflectionExponent(DEFAULT_GLOSSY_REFLECTION_EXPONENT);
    setGlossyReflectionSampler(new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS));
}

vec3 GlossyReflector::shade(Shade &shade) {
    vec3 color = Phong::shade(shade);
    dvec3 in;
    dvec3 out = -shade.ray.direction;
    float probDensity;

    vec3 brdf = glossySpecularBRDF->sampleF(shade, in, out, &probDensity);
    Ray reflRay = Ray(shade.hitPoint, in);
    color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1) *
             float(dot(shade.normal, in)) / probDensity;

    return color;
}

vec3 GlossyReflector::globalShade(Shade &shade) {
    vec3 color = Phong::shade(shade);
    
    dvec3 in, out = -shade.ray.direction;
    float probDensity;
    vec3 brdf = glossySpecularBRDF->sampleF(shade, in, out, &probDensity);
    Ray reflRay = Ray(shade.hitPoint, in);

    if (shade.depth == 0)
        color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 2)
                 * float(dot(shade.normal, in)) / probDensity;
    else
        color += brdf * shade.world.tracerP->traceRay(reflRay, shade.depth + 1) 
                 * float(dot(shade.normal, in)) / probDensity;
    
    return color;
}

GlossyReflector::~GlossyReflector() {
    Phong::~Phong();
    delete glossySpecularBRDF;
}
