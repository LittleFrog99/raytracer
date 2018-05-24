#include "bssrdf.h"

BSSRDFTable::BSSRDFTable(int num_albedo_samples, int num_radius_samples)
    : nAlbedo(num_albedo_samples), nRadius(num_radius_samples),
      albedo(new float[nAlbedo]), radius(new float[nRadius]), profile(new float[nAlbedo * nRadius]),
      effAlbedo(new float[nAlbedo]), profileCDF(new float[nAlbedo * nRadius])
{}

float BSSRDF::fresnelReflFactor(float cosThetaI, float eta) {
    float cosThetaT = sqrt(1.0 - (1.0 - cosThetaI * cosThetaI) / (eta * eta));
    float rPara = (eta * cosThetaI - cosThetaT) / (eta * cosThetaI + cosThetaT);
    float rPerp = (cosThetaI - eta * cosThetaT) / (cosThetaI + eta * cosThetaT);

    return 0.5 * (rPara * rPara + rPerp * rPerp);
}

float BSSRDF::fresnelMoment1(float eta) {
    float eta2 = eta * eta, eta3 = eta2 * eta, eta4 = eta3 * eta, eta5 = eta4 * eta;
    if (eta < 1)
        return 0.45966f - 1.73965f * eta + 3.37668f * eta2 - 3.904945 * eta3 +
               2.49277f * eta4 - 0.68441f * eta5;
    else
        return -4.61686f + 11.1136f * eta - 10.4646f * eta2 + 5.11455f * eta3 -
               1.27198f * eta4 + 0.12746f * eta5;
}

float BSSRDF::fresnelMoment2(float eta) {
    float eta2 = eta * eta, eta3 = eta2 * eta, eta4 = eta3 * eta, eta5 = eta4 * eta;
    if (eta < 1)
        return 0.27614f - 0.87350f * eta + 1.12077f * eta2 - 0.65095f * eta3 +
               0.07883f * eta4 + 0.04860f * eta5;
    else {
        float r_eta = 1 / eta, r_eta2 = r_eta * r_eta, r_eta3 = r_eta2 * r_eta;
        return -547.033f + 45.3087f * r_eta3 - 218.725f * r_eta2 +
               458.843f * r_eta + 404.557f * eta - 189.519f * eta2 +
               54.9327f * eta3 - 9.00603f * eta4 + 0.63942f * eta5;
    }
}

float BSSRDF::phaseHG(float cosTheta, float g) {
    float denom = 1 + g * g + 2 * g * cosTheta;
    return 1.0 / (4.0 * PI) * (1 - g * g) / (denom * sqrt(denom));
}

void BSSRDF::tangentSpace(const dvec3 &w, dvec3 &u, dvec3 &v) {
    v = normalize(cross(UP_VECTOR, w));
    u = cross(v, w);
}

float BSSRDF::beamDiffusion(float scatter, float absorp, float g, float eta, float radius) 
{
    static const int NUM_SAMPLES = 100;
    float Ed = 0;

    // Precompute information for dipole integrand
    // Compute reduced scattering coefficients (σ's, σ't) and albedo (ρ') 
    float rdScatter = scatter * (1 - g);
    float rdExtinc = absorp + rdScatter;
    float rdAlbedo = rdScatter / rdExtinc;
    // Compute non-classical diffusion coefficient D_g (Grosjean's solution) (P924 15.24)
    float D_g = (2 * absorp + rdScatter) / (3 * rdExtinc * rdExtinc);
    // Compute effective transport coefficient (σtr) based on D_g
    float effTrans = sqrt(absorp / D_g);
    // Determine linear extrapolation distance (ze) (P926 15.28)
    float fm1 = fresnelMoment1(eta), fm2 = fresnelMoment2(eta);
    float extrapDist = -2 * D_g * (1 + 3 * fm2) / (1 - 2 * fm1);
    // Determine exitance scale factors (P927 15.31 & 15.32)
    float cFluence = 0.25f * (1 - 2 * fm1), cIrrad = 0.5f * (1 - 3 * fm2);

    for (int i = 0; i < NUM_SAMPLES; ++i) {
        // Sample real point source (zr)
        float zReal = -log(1 - (i + 0.5f) / NUM_SAMPLES) / rdExtinc;
        float zVirt = -zReal + 2 * extrapDist;
        float distReal = sqrt(radius * radius + zReal * zReal);
        float distVirt = sqrt(radius * radius + zVirt * zVirt);

        // Compute dipole fluence rate (φD) (P923 15.20 & P926 15.25)
        float fluenceRate = 1.0 / (4 * PI) / D_g * (exp(-effTrans * distReal) / distReal 
                                                    - exp(-effTrans * distVirt) / distVirt);
        // Compute dipole vector irradiance (-ED·n) (P926 15.27)
        float vecIrrad = 1.0 / (4 * PI) * 
            (zReal * (1 + effTrans * distReal) * exp(-effTrans * distReal) / pow(distReal, 3) 
             - zVirt * (1 + effTrans * distVirt) * exp(-effTrans * distVirt) / pow(distVirt, 3));

        // Add contribution from dipole for depth (zr) to Ed
        float E = fluenceRate * cFluence + vecIrrad * cIrrad;
        float kappa = 1 - exp(-2 * rdExtinc * (distReal + zReal));
        Ed += kappa * rdAlbedo * rdAlbedo * E;
    }
    return Ed / NUM_SAMPLES;
}

float BSSRDF::beamSingleScattering(float scatter, float absorp, float g, float eta, float radius) 
{
    // Compute material parameters and minimum t below the critical angle
    float extinc = absorp + scatter, albedo = scatter / extinc; // don't need reduced parameters
    float tCrit = radius * sqrt(eta * eta - 1);
    float Ess = 0;
    static const int NUM_SAMPLES = 100;
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        float ti = tCrit - log(1 - (i + 0.5f) / NUM_SAMPLES) / extinc;
        // Determine length of connecting segment and cos(θo)
        float dist = sqrt(radius * radius + ti * ti);
        float cosThetaO = ti / dist;
        // Add contribution of single scattering at depth t
        Ess += albedo * exp(-extinc * (dist + tCrit)) / (dist * dist) * phaseHG(cosThetaO, g) 
                   * (1 - fresnelReflFactor(cosThetaO, eta)) * abs(cosThetaO);
    }
    return Ess / NUM_SAMPLES;
}

void BSSRDF::genProfileTable(float g, float eta, BSSRDFTable *table)
{
    // Choose radius values of the diffusion profile discretization
    table->radius[0] = 0;
    table->radius[1] = 2.5e-3f;
    for (int i = 2; i < table->nRadius; ++i) {
        table->radius[i] = table->radius[i - 1] * 1.15f;
        // cout << table->radius[i] << endl;
    }

    // Choose albedo values of the diffusion profile discretization
    for (int i = 0; i < table->nAlbedo; ++i) 
        table->albedo[i] =
            (1 - exp(-8 * i / (float)(table->nAlbedo - 1))) /
            (1 - exp(-8));

    for (int i = 0; i < table->nAlbedo; i++) {
        // Compute the diffusion profile for the i th albedo sample
        // Compute scattering profile for chosen albedo 
        for (int j = 0; j < table->nRadius; ++j) {
            float albedo = table->albedo[i], radius = table->radius[j];
            table->profile[i * table->nRadius + j] =
                2 * PI * radius * (beamSingleScattering(albedo, 1 - albedo, g, eta, radius) +
                                   beamDiffusion(albedo, 1 - albedo, g, eta, radius));
        }

        // Compute effective albedo and CDF for importance sampling
        table->effAlbedo[i] =
            Interpolation::integrateCatmullRom(table->nRadius, table->radius.get(), 
                &table->profile[i * table->nRadius], &table->profileCDF[i * table->nRadius]);
    }
}
