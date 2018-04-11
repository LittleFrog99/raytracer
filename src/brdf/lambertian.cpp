#include "lambertian.h"
#include "sampler/multijittered.h"

Lambertian::Lambertian(float intensity, vec3 color) : 
    intensity(intensity), color(color)
{
    setSampler(new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS));
}


vec3 Lambertian::calcBRDF(Shade &shade, dvec3 &wi, dvec3 &wo) {
    return float(intensity * INV_PI) * color;
}

vec3 Lambertian::calcReflectance(Shade &shade, dvec3 &wo) {
    return intensity * color;
}

vec3 Lambertian::sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *pdf) {
    dvec3 w = shade.normal;
    dvec3 v = normalize(cross(UP_VECTOR, w));
    dvec3 u = cross(v, w);

    dvec3 samplePt = samplerP->sampleUnitHemisphere();
    in = normalize(samplePt.x * u + samplePt.y * v + samplePt.z * w);

    if (pdf) *pdf = INV_PI * dot(shade.normal, in);
    return float(INV_PI * intensity) * color;
}
