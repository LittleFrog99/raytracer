#include "tabulated.h"

Tabulated::Tabulated(float eta, vec3 absorp, vec3 scatter, BSSRDFTable *table)
    : Separable(eta), table(table)
{
    extinction = absorp + scatter;
    for (int i = 0; i < 3; i++) 
        albedo[i] = extinction[i] == 0 ? 0 : (scatter[i] / extinction[i]);
}

vec3 Tabulated::calcSr(float distance) {
    vec3 Sr;
    for (int ch = 0; ch < 3; ch++) {
        // Convert distance into unitless optical radius
        float rOpt = distance * extinction[ch];
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
    Sr *= extinction * extinction;
    clamp(Sr, vec3(), vec3(INFINITY));
}
