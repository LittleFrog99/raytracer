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
    return commonShade(shade, shade.depth + 1);
}

vec3 Reflective::globalShade(Shade &shade) {
    return commonShade(shade, shade.depth == 0 ? shade.depth + 2 : shade.depth + 1);
}

vec3 Reflective::commonShade(Shade &shade, int depth) {
    vec3 color = Phong::shade(shade);

    dvec3 in;
    dvec3 out = -shade.ray.direction;
    vec3 brdf = reflBRDF->sampleBRDF(shade, in, out);
    Ray reflRay = Ray(shade.hitPoint, in);

    color += brdf * shade.world.tracerP->traceRay(reflRay, depth);

    return color;
}


Reflective::~Reflective() {
    Phong::~Phong();
    delete reflBRDF;
}
