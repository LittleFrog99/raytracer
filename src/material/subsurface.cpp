#include "subsurface.h"
#include "core/world.h"

Subsurface::Subsurface(float eta, vec3 albedo, vec3 scatter) {
    SSS = new Tabulated(this, eta, albedo, scatter, &table);
}

vec3 Subsurface::pathShade(Shade &po) {
    // Importance sample the BSSRDF
    Shade pi(po.world);
    vec3 L;
    float pdf;
    vec3 S = SSS->sampleS(po, rndm.randomDouble(), dvec2(rndm.randomDouble(), rndm.randomDouble()), pi, &pdf);
    if (S == Color::BLACK || pdf == 0) return Color::BLACK;
    // Account for the direct illumination
    for (auto light : po.world.lights) {
        dvec3 wi = light->calcDirection(po);
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
    dvec3 wi;
    float Sw = SSS->sampleSw(pi, wi, &pdf);
    Ray newRay(pi.hitPoint, wi);
    L += SSS->calcS(po, pi, wi) * po.world.tracerP->traceRay(newRay, po.depth + 1) 
        * float(dot(wi, pi.normal)) / pdf;
    return L;
}
