#include "glossyreflector.h"
#include "sampler/multijittered.h"
#include "core/world.h"

GlossyReflector::GlossyReflector(vec3 color, float amb_int, float diff_int, 
    float spec_int, float refl_int) : Phong(color, amb_int, diff_int, spec_int) 
{
    glossySpecularBRDF = new GlossySpecular();
    setGlossyReflectionIntensity(refl_int);
    setGlossyReflectionColor(color);
    setGlossyReflectionExponent(1000.0f);
    setGlossyReflectionSampler(new MultiJittered(256, 2));
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
