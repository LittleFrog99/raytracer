#include "perfectspecular.h"

vec3 PerfectSpecular::sampleF(Shade &shade, glm::dvec3 &in, 
                              glm::dvec3 &out, float *pdf) 
{
    float nDotOut = dot(shade.normal, out);
    in = reflect(-out, shade.normal);
    if (pdf) *pdf = dot(shade.normal, in);
    return intensity * color;
}
