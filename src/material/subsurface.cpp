#include "subsurface.h"
#include "core/world.h"
#include "bsdf/lambertian.h"
#include "bsdf/fresnelreflector.h"

Subsurface::Subsurface(float scale, float eta, float g, vec3 absorp, vec3 scatter) {
    table = BSSRDFTable(100, 64);
    SSS = new Tabulated(this, eta, g, scale * absorp, scale * scatter, &table);
    reflBRDF = new FresnelReflector(Color::WHITE, eta, 1.0f);
}

vec3 Subsurface::pathShade(Shade &po) {
    vec3 L;
    dvec3 wi, wo = -po.ray.direction;
    // Calculate BRDF
    vec3 brdf = reflBRDF->sampleBRDF(po, wi, wo);
    if (brdf != Color::BLACK && dot(wo, po.normal) > 0) {
        Ray reflRay = Ray(po.hitPoint, wi);
        L += brdf * po.world.tracerP->traceRay(reflRay, po.depth + 1);
    }
    // Importance sample the BSSRDF
    Shade pi(po.world);
    float pdfSp, pdfSw;
    double u1 = rndm.randomDouble();
    dvec2 u2(rndm.randomDouble(), rndm.randomDouble());
    vec3 S = SSS->sampleS(po, u1, u2, pi, &pdfSp); // (1 - Ft) * Sp, no Sw
    if (S != Color::BLACK && pdfSp != 0) {
        // Account for the direct illumination
        for (auto light : po.world.lights) {
            dvec3 wi = light->calcDirection(pi);
            float nDotWi = dot(pi.normal, wi);
            if (nDotWi > 0.0) {
                bool inShadow = false;
                if (light->castShadow()) {
                    Ray shadowRay(pi.hitPoint, wi);
                    inShadow = light->inShadow(shadowRay, pi);
                }
                if (!inShadow) 
                    L += S * SSS->calcSw(pi, wi) * light->incidRadiance(pi) * light->geoTerm(pi) 
                           * nDotWi / (light->calcPdf(pi) * pdfSp);
            }
        }
        // And the indirect one
        float Sw = SSS->sampleSw(pi, wi, &pdfSw);
        Ray newRay(pi.hitPoint, wi);
        L += S * Sw * po.world.tracerP->traceRay(newRay, po.depth + 1) 
            * float(dot(wi, pi.normal)) / (pdfSp * pdfSw);
    }
    return L;
}
