#include "perfectspecular.h"
#include "sampler/multijittered.h"

PerfectSpecular::PerfectSpecular() {
    setSampler(new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS));
}

PerfectSpecular::PerfectSpecular(float intensity, vec3 color) 
    : color(color), intensity(intensity) 
{
    PerfectSpecular();
}

vec3 PerfectSpecular::sampleBRDF(Shade &shade, glm::dvec3 &in, 
                              glm::dvec3 &out, float *pdf) 
{
    float nDotOut = dot(shade.normal, out);
    in = reflect(-out, shade.normal);
    if (pdf) *pdf = dot(shade.normal, in);
    return intensity * getColor(shade);
}
