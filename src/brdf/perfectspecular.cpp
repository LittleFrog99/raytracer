#include "perfectspecular.h"

vec3 PerfectSpecular::sampleF(Shade &shade, glm::dvec3 &in, 
                              glm::dvec3 &out, float *prob_den) 
{
    float nDotOut = dot(shade.normal, out);
    in = reflect(-out, shade.normal);
    return intensity * color;
}
