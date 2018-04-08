#include "specular.h"

vec3 Specular::calcBRDF(Shade &shade, dvec3 &in, dvec3 &out) {
    double nDotWi = dot(shade.normal, in);
    dvec3 ref = -in + 2.0 * nDotWi * shade.normal;
    float rDotWo = dot(ref, out);
    return rDotWo > 0.0 ? (intensity * powf(rDotWo, exponent) * color) : vec3();
}

vec3 Specular::calcReflectance(Shade &shade, dvec3 &out) {
    return vec3();
}
