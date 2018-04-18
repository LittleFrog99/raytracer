#include "dielectric.h"
#include "core/world.h"

Dielectric::Dielectric(vec3 color, float amb_int, float diff_int, float spec_int, 
                       float eta_in, float eta_out, vec3 filter_in, vec3 filter_out)
    : Phong(color, amb_int, diff_int, spec_int), etaIn(eta_in), etaOut(eta_out),
      filterIn(filter_in), filterOut(filter_out)
{
    fresnelBRDF = new FresnelReflector(color, eta_in, eta_out);
    fresnelBTDF = new FresnelTransmitter(color, eta_in, eta_out);
}

vec3 Dielectric::shade(Shade &shade) {
    return commonShade(shade, shade.depth + 1);
}

vec3 Dielectric::globalShade(Shade &shade) {
    return commonShade(shade, shade.depth == 0 ? shade.depth + 2 : shade.depth + 1);
}

vec3 Dielectric::commonShade(Shade &shade, int depth) {
    vec3 color = Phong::shade(shade);

    dvec3 in, out = -shade.ray.direction;
    vec3 brdf = fresnelBRDF->sampleBRDF(shade, in, out);
    Ray reflRay = Ray(shade.hitPoint, in);
    float nDotIn = dot(shade.normal, in);

    double t;
    vec3 reflColor, transColor;
    reflColor = shade.world.tracerP->traceRay(reflRay, depth, &t);

    if (fresnelBTDF->isTIR(shade))
        color += reflColor * pow(nDotIn < 0.0 ? filterIn : filterOut, vec3(t)); // reflection factor = 1
    else { // no total internal reflection
        dvec3 trans;
        vec3 btdf = fresnelBTDF->sampleBTDF(shade, trans, out);
        Ray transRay = Ray(shade.hitPoint, trans);
        float nDotT = dot(shade.normal, trans);

        color += brdf * reflColor * pow(nDotIn < 0.0 ? filterIn : filterOut, vec3(t));
        
        transColor = shade.world.tracerP->traceRay(transRay, depth, &t);
        color += btdf * transColor * pow(nDotT > 0.0 ? filterIn : filterOut, vec3(t));
    }

    return color;
}
