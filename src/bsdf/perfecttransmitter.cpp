#include "perfecttransmitter.h"
#include "sampler/multijittered.h"

PerfectTransmitter::PerfectTransmitter(float intensity, float ior)
    : ior(ior), intensity(intensity) 
{
    setSampler(new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS));
}

bool PerfectTransmitter::isTIR(Shade &shade) {
    dvec3 out = -shade.ray.direction;
    float cosThetaI = dot(shade.normal, out);
    float eta = cosThetaI > 0.0 ? ior : 1.0 / ior;

    return 1.0 - (1.0 - cosThetaI * cosThetaI) / (eta * eta) < 0.0;
}

vec3 PerfectTransmitter::sampleBTDF(Shade &shade, dvec3 &trans, dvec3 &out) {
    dvec3 normal = shade.normal;
    double cosThetaI = dot(normal, out);
    double eta = ior;
    if (cosThetaI < 0.0) {
        cosThetaI = -cosThetaI;
        normal = -normal;
        eta = 1.0 / eta;
    }

    double cosThetaT = sqrt(1.0 - (1.0 - cosThetaI * cosThetaI) / (eta * eta));
    trans = -out / eta - (cosThetaT - cosThetaI / eta) * normal;
    return float(intensity / fabs(dot(normal, trans)) / (eta * eta)) * color;
}
