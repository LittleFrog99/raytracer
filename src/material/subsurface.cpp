#include "subsurface.h"
#include "core/world.h"
#include "bsdf/lambertian.h"

Subsurface::Subsurface(float eta, float g, vec3 absorp, vec3 scatter) {
    table = BSSRDFTable(100, 64);
    SSS = new Tabulated(this, eta, g, absorp, scatter, &table);
}

vec3 Subsurface::pathShade(Shade &po) {
    // Calculate BRDF
    vec3 L;
    dvec3 wi;
    dvec3 wo = -po.ray.direction;
    if (BRDF) { 
        float pdf;
        vec3 brdf = BRDF->sampleBRDF(po, wi, wo, &pdf);
        Ray reflRay = Ray(po.hitPoint, wi);
        L += brdf * po.world.tracerP->traceRay(reflRay, po.depth + 1) * float(dot(wo, po.normal)) / pdf;
    }
    // Importance sample the BSSRDF
    Shade pi(po.world);
    float pdf, pdfSw;
    double u1 = rndm.randomDouble();
    dvec2 u2(rndm.randomDouble(), rndm.randomDouble());
    vec3 S = SSS->sampleS(po, u1, u2, pi, &pdf);
    if (S == Color::BLACK || pdf == 0) return Color::BLACK;
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
                L += SSS->calcS(po, pi, wi) * light->incidRadiance(pi) * light->geoTerm(pi) * nDotWi / (light->probDenFunc(pi) * pdf);
        }
    }
    // And the indirect one
    float Sw = SSS->sampleSw(pi, wi, &pdfSw);
    Ray newRay(pi.hitPoint, wi);
    L += SSS->calcS(po, pi, wi) * po.world.tracerP->traceRay(newRay, po.depth + 1) 
        * float(dot(wi, pi.normal)) / (pdf * pdfSw);
    return 4.0f * L;
}
