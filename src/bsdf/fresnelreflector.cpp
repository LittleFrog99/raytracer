#include "fresnelreflector.h"
#include "sampler/multijittered.h"

FresnelReflector::FresnelReflector(vec3 color, float eta_in, float eta_out)
    : PerfectSpecular() {
    setColor(color);
    setEta(eta_in, eta_out);
}

vec3 FresnelReflector::sampleBRDF(Shade &shade, dvec3 &in, const dvec3 &out, float *pdf) {
    PerfectSpecular::sampleBRDF(shade, in, out);
    return fresnelReflFactor(shade) * getColor(shade); 
}

float FresnelReflector::fresnelReflFactor(Shade &shade) {
    dvec3 normal = shade.normal;
    float nDotD = dot(-shade.ray.direction, normal);
    float eta = etaIn / etaOut;
    if (nDotD < 0.0) {
        normal = -normal;
        nDotD = -nDotD;
        eta = 1.0 / eta;
    }

    float cosThetaI = nDotD / length(shade.ray.direction); // in case of instance transformation
    float cosThetaT = sqrt(1.0 - (1.0 - cosThetaI * cosThetaI) / (eta * eta));
    float rPara = (eta * cosThetaI - cosThetaT) / (eta * cosThetaI + cosThetaT);
    float rPerp = (cosThetaI - eta * cosThetaT) / (cosThetaI + eta * cosThetaT);

    return 0.5 * (rPara * rPara + rPerp * rPerp);
}
