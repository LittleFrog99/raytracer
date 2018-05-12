#include "tabulated.h"

Tabulated::Tabulated(Material *mat_ptr, float eta, vec3 absorp, vec3 scatter, BSSRDFTable *table)
    : Separable(mat_ptr, eta), table(table)
{
    extinc = absorp + scatter;
    for (int i = 0; i < 3; i++) 
        albedo[i] = extinc[i] == 0 ? 0 : (scatter[i] / extinc[i]);
}

vec3 Tabulated::calcSr(float distance) {
    vec3 Sr;
    for (int ch = 0; ch < 3; ch++) {
        // Convert distance into unitless optical radius
        float rOpt = distance * extinc[ch];
        // Compute spline weights to interpolate BSSRDF on channel ch
        int albedoOff, radiusOff;
        float albedoWeights[4], radiusWeights[4];
        if (!Math::catmullRomWeights(table->albedo.size(), &table->albedo[0], albedo[ch], 
            &albedoOff, albedoWeights) ||
            !Math::catmullRomWeights(table->radius.size(), &table->radius[0], rOpt, 
            &radiusOff, radiusWeights))
            continue;
        // Set BSSRDF distance term sr[ch] using tensor spline interpolation
        float srCh = 0;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                float weight = albedoWeights[i] * radiusWeights[i];
                if (weight != 0)
                    srCh += weight * table->evaluateProfile(albedoOff + i, radiusOff + j);
            }
        // Cancel marginal PDF factor from tabulated BSSRDF profile
        if (rOpt != 0) srCh /= (2 * PI * rOpt);
        Sr[ch] = srCh;
    }
    // Transform BSSRDF value into world space units
    Sr *= extinc * extinc;
    return clamp(Sr, vec3(), vec3(INFINITY));
}

float Tabulated::sampleSr(int channel, double u) {
    if (extinc[channel] == 0) return -1;
    return Math::sampleCatmullRom2D(table->albedo.size(), table->radius.size(), 
        &table->albedo[0], &table->radius[0], &table->profile[0], &table->profileCDF[0],
        albedo[channel], u);
}

float Tabulated::pdfSr(int channel, double radius) {
    // Convert radius into unitless optical radius
    float rOpt = radius * extinc[channel];
    // Compute spline weights to interpolate BSSRDF density on channel
    int albedoOff, radiusOff;
    float albedoWeights[4], radiusWeights[4];
    if (!Math::catmullRomWeights(table->albedo.size(), &table->albedo[0], albedo[channel],
                                 &albedoOff, albedoWeights) ||
        !Math::catmullRomWeights(table->radius.size(), &table->radius[0], rOpt,
                                 &radiusOff, radiusWeights))
        return 0.0f;
    
    // Return BSSRDF profile density for channel
    float srCh = 0, effAlbedo = 0;
    for (int i = 0; i < 4; i++) {
        if (albedoWeights[i] == 0) continue;
        effAlbedo += table->effAlbedo[albedoOff + i] * albedoWeights[i];
        for (int j = 0; j < 4; j++) {
            if (radiusWeights[j] == 0) continue;
            srCh += albedoWeights[i] * radiusWeights[j] * table->evaluateProfile(albedoOff + i, radiusOff + j);
        }
    }
    // Cancel marginal PDF factor from tabulated BSSRDF profile
    if (rOpt != 0) srCh /= (2 * PI * rOpt);
    return std::max(0.0f, srCh * extinc[channel] * extinc[channel] / effAlbedo);
}

