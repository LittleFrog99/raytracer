#include "specular.h"
#include "sampler/multijittered.h"

Specular::Specular(float intensity, vec3 color, float exponent) : 
    intensity(intensity), color(color), exponent(exponent) 
{
    setSampler(new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS), exponent);
}

vec3 Specular::calcBRDF(Shade &shade, dvec3 &in, dvec3 &out) {
    double nDotWi = dot(shade.normal, in);
    dvec3 ref = -in + 2.0 * nDotWi * shade.normal;
    float rDotWo = dot(ref, out);
    return rDotWo > 0.0 ? (intensity * powf(rDotWo, exponent) * color) : vec3();
}

vec3 Specular::calcReflectance(Shade &shade, dvec3 &out) {
    return vec3();
}

vec3 Specular::sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *prob_den) {
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