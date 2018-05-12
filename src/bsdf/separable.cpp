#include "separable.h"

vec3 Separable::calcS(Shade &po, Shade &pi, dvec3 &wi) {
    float cosTheta = fabs(dot(po.hitPoint, -po.ray.direction));
    float ft = 1 - fresnelReflFactor(cosTheta);
    return ft * calcSw(pi, wi) * calcSp(po, pi);
}

float Separable::calcSw(Shade &pi, dvec3 &wi) {
    float c = 1.0 - 2.0 * fresnelMoment1(1.0 / eta);
    float cosTheta = fabs(dot(pi.hitPoint, wi));
    return (1 - fresnelReflFactor(cosTheta)) / (c * PI);
}

vec3 Separable::calcSp(Shade &po, Shade &pi) {
    return calcSr(distance(po.hitPoint, pi.hitPoint));
}

float Separable::fresnelReflFactor(float cos_theta) {
    float cosThetaI = cos_theta; 
    float cosThetaT = sqrt(1.0 - (1.0 - cosThetaI * cosThetaI) / (eta * eta));
    float rPara = (eta * cosThetaI - cosThetaT) / (eta * cosThetaI + cosThetaT);
    float rPerp = (cosThetaI - eta * cosThetaT) / (cosThetaI + eta * cosThetaT);

    return 0.5 * (rPara * rPara + rPerp * rPerp);
}

vec3 Separable::sampleS(Shade &po, double u1, const dvec2 &u2, Shade &pi, double *pdf)
{
    vec3 Sp = sampleSp(po, u1, u2, pi, pdf);
    if (Sp != Color::BLACK) {
        // Initialize material model at sampled surface interaction
    }
    return Sp;
}

vec3 Separable::sampleSp(Shade &po, double u1, const dvec2 &u2, Shade &pi, double *pdf)
{
    // Choose projection axis for BSSRDF sampling
    // Choose spectral channel for BSSRDF sampling
    // Sample BSSRDF profile in polar coordinates
    // Compute BSSRDF profile bounds and intersection height
    // Compute BSSRDF sampling ray segment
    // Intersect BSSRDF sampling ray against the scene geometry
    // Randomly choose one of several intersections
    // Compute sample PDF and return the spatial term Sp
}
