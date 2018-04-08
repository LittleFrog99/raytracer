#include "glossyspecular.h"

vec3 GlossySpecular::sampleF(Shade &shade, glm::dvec3 &in, glm::dvec3 &out, float *prob_den) {
    dvec3 refl = reflect(-out, shade.normal);
    dvec3 w = refl;
    dvec3 u = normalize(cross(UP_VECTOR, w));
    dvec3 v = cross(u, w);

    dvec3 samplePt = samplerP->sampleUnitHemisphere();
    in = dot(shade.normal, out) > 0.0 ? (samplePt.x * u + samplePt.y * v + samplePt.z * w)
                                      : (-samplePt.x * u - samplePt.y * v + samplePt.z * w);

    float phongLobe = pow(dot(refl, in), exponent);
    *prob_den = phongLobe * dot(shade.normal, in);
    return intensity * phongLobe * color;
}
