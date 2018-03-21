#include "glossyspecular.h"

vec3 GlossySpecular::calcBRDF(Shade &shade, dvec3 &in, dvec3 &out) {
    double nDotWi = dot(shade.normal, in);
    dvec3 ref = -in + 2.0 * nDotWi * shade.normal;
    float rDotWo = dot(ref, out);
    return rDotWo > 0.0 ? (intensity * powf(rDotWo, exponent) * color) : vec3();
}

vec3 GlossySpecular::calcReflectance(Shade &shade, dvec3 &out) {
    return vec3();
}

vec3 GlossySpecular::sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *reflect) {
    float nDotWo = dot(shade.normal, out);
    dvec3 r = -out + 2.0 * nDotWo * shade.normal;

    dvec3 w = r;
    dvec3 u = normalize(cross(UP_VECTOR, w));
    dvec3 v = cross(u, w);

    dvec3 sp = samplerP->sampleUnitHemisphere();
    in = dot(shade.normal, in) > 0 ?
        (sp.x * u + sp.y * v + sp.z * w) : 
        (-sp.x * u - sp.y * v + sp.z * w);
    
    float phongLobe = powf(dot(r, in), exponent);
    *reflect = phongLobe * dot(shade.normal, in);

    return intensity * phongLobe * color;
}