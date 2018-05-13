#include "bssrdf.h"

BSSRDFTable::BSSRDFTable(int num_albedo_samples, int num_radius_samples) 
    : nAlbedo(num_albedo_samples), nRadius(num_radius_samples)
{
    albedo.reserve(nAlbedo);
    radius.reserve(nRadius);
    profile.reserve(nAlbedo * nRadius);
    effAlbedo.reserve(nAlbedo);
    profileCDF.reserve(nAlbedo * nRadius);
}

float BSSRDF::fresnelReflFactor(float cos_theta, float eta) {
    float cosThetaI = cos_theta; 
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

float BSSRDF::beamDiffusionMS(float scatter, float absorp, float g, float eta, float r)
{
    const int nSamples = 100;
    float Ed = 0;
    // Precompute information for dipole integrand
    // Compute reduced scattering coefficients $\sigmaps, \sigmapt$ and albedo
    // $\rhop$
    float sigmap_s = scatter * (1 - g);
    float sigmap_t = absorp + sigmap_s;
    float rhop = sigmap_s / sigmap_t;
    // Compute non-classical diffusion coefficient $D_\roman{G}$ using
    // Equation (15.24)
    float D_g = (2 * absorp + sigmap_s) / (3 * sigmap_t * sigmap_t);
    // Compute effective transport coefficient $\sigmatr$ based on $D_\roman{G}$
    float sigma_tr = std::sqrt(absorp / D_g);
    // Determine linear extrapolation distance $\depthextrapolation$ using
    // Equation (15.28)
    float fm1 = fresnelMoment1(eta), fm2 = fresnelMoment2(eta);
    float ze = -2 * D_g * (1 + 3 * fm2) / (1 - 2 * fm1);
    // Determine exitance scale factors using Equations (15.31) and (15.32)
    float cPhi = .25f * (1 - 2 * fm1), cE = .5f * (1 - 3 * fm2);
    for (int i = 0; i < nSamples; ++i) {
        // Sample real point source depth $\depthreal$
        float zr = -std::log(1 - (i + .5f) / nSamples) / sigmap_t;
        // Evaluate dipole integrand $E_{\roman{d}}$ at $\depthreal$ and add to
        // _Ed_
        float zv = -zr + 2 * ze;
        float dr = std::sqrt(r * r + zr * zr), dv = std::sqrt(r * r + zv * zv);
        // Compute dipole fluence rate $\dipole(r)$ using Equation (15.27)
        float phiD = 1.0 / (4 * PI) / D_g * (std::exp(-sigma_tr * dr) / dr 
            - std::exp(-sigma_tr * dv) / dv);
        // Compute dipole vector irradiance $-\N{}\cdot\dipoleE(r)$ using
        // Equation (15.27)
        float EDn = 1.0 / (4 * PI) * (zr * (1 + sigma_tr * dr) 
            * std::exp(-sigma_tr * dr) / (dr * dr * dr) - zv * (1 + sigma_tr * dv) 
            * std::exp(-sigma_tr * dv) / (dv * dv * dv));
        // Add contribution from dipole for depth $\depthreal$ to _Ed_
        float E = phiD * cPhi + EDn * cE;
        float kappa = 1 - std::exp(-2 * sigmap_t * (dr + zr));
        Ed += kappa * rhop * rhop * E;
    }
    return Ed / nSamples;
}

float BSSRDF::beamDiffusionSS(float scatter, float absorp, float g, float eta, float r)
{
    // Compute material parameters and minimum $t$ below the critical angle
    float sigma_t = absorp + scatter, rho = scatter / sigma_t;
    float tCrit = r * std::sqrt(eta * eta - 1);
    float Ess = 0;
    const int nSamples = 100;
    for (int i = 0; i < nSamples; ++i) {
        // Evaluate single scattering integrand and add to _Ess_
        float ti = tCrit - std::log(1 - (i + .5f) / nSamples) / sigma_t;

        // Determine length $d$ of connecting segment and $\cos\theta_\roman{o}$
        float d = std::sqrt(r * r + ti * ti);
        float cosThetaO = ti / d;

        // Add contribution of single scattering at depth $t$
        Ess += rho * exp(-sigma_t * (d + tCrit)) / (d * d) *
               phaseHG(cosThetaO, g) * (1 - fresnelReflFactor(-cosThetaO, eta)) *
               abs(cosThetaO);
    }
    return Ess / nSamples;
}

void BSSRDF::calcBeamDiffusion(float g, float eta, BSSRDFTable *table)
{
    // Choose radius values of the diffusion profile discretization
    table->radius[0] = 0;
    table->radius[1] = 2.5e-3f;
    for (int i = 2; i < table->nRadius; ++i)
        table->radius[i] = table->radius[i - 1] * 1.2f;

    // Choose albedo values of the diffusion profile discretization
    for (int i = 0; i < table->nAlbedo; ++i)
        table->albedo[i] =
            (1 - std::exp(-8 * i / (float)(table->nAlbedo - 1))) /
            (1 - std::exp(-8));

    for (int i = 0; i < table->nAlbedo; i++) {
        // Compute the diffusion profile for the _i_th albedo sample
        // Compute scattering profile for chosen albedo $\rho$
        for (int j = 0; j < table->nRadius; ++j) {
            float rho = table->albedo[i], r = table->radius[j];
            table->profile[i * table->nRadius + j] =
                2 * PI * r * (beamDiffusionSS(rho, 1 - rho, g, eta, r) +
                              beamDiffusionMS(rho, 1 - rho, g, eta, r));
        }

        // Compute effective albedo $\rho_{\roman{eff}}$ and CDF for importance sampling
        table->effAlbedo[i] =
            Interpolation::integrateCatmullRom(table->nRadius, &table->radius[0],
                                &table->profile[i * table->nRadius],
                                &table->profileCDF[i * table->nRadius]);
    }
}
