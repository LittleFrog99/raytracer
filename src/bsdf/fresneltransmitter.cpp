#include "fresneltransmitter.h"
#include "sampler/multijittered.h"

FresnelTransmitter::FresnelTransmitter(vec3 color, float eta_in, float eta_out) {
    this->color = color;
    setEta(eta_in, eta_out);
    setSampler(new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS));
}

bool FresnelTransmitter::isTIR(Shade &shade) {
    dvec3 out = -shade.ray.direction;
    float cosThetaI = dot(shade.normal, out);
    float eta = cosThetaI > 0.0 ? etaIn / etaOut : etaOut / etaIn;

    return 1.0 - (1.0 - cosThetaI * cosThetaI) / (eta * eta) < 0.0;
}

vec3 FresnelTransmitter::sampleBTDF(Shade &shade, dvec3 &trans, dvec3 &out) {
    dvec3 normal = shade.normal;
    double cosThetaI = dot(normal, out);
    double eta = etaIn / etaOut;
    if (cosThetaI < 0.0) {
        cosThetaI = -cosThetaI;
        normal = -normal;
        eta = 1.0 / eta;
    }

    double cosThetaT = sqrt(1.0 - (1.0 - cosThetaI * cosThetaI) / (eta * eta));
    trans = -out / eta - (cosThetaT - cosThetaI / eta) * normal;
    return float(fresnelTransFactor(shade) / fabs(dot(normal, trans)) / (eta * eta)) * color;
}

float FresnelTransmitter::fresnelTransFactor(Shade &shade) {
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

    return 1.0 - 0.5 * (rPara * rPara + rPerp * rPerp);
}
