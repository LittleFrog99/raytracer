#include "separable.h"
#include "core/world.h"
#include "sampler/multijittered.h"

Separable::Separable(Material *mat_ptr, float eta) : materialP(mat_ptr), eta(eta)
{
    smplrP = new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS);
    smplrP->mapSamplesToHemisphere();
}

vec3 Separable::calcS(Shade &po, Shade &pi, dvec3 &wi) {
    float cosTheta = fabs(dot(po.hitPoint, -po.ray.direction));
    float ft = 1 - fresnelReflFactor(cosTheta, eta);
    return ft * calcSw(pi, wi) * calcSp(po, pi);
}

float Separable::calcSw(Shade &pi, dvec3 &wi) {
    float c = 1.0 - 2.0 * fresnelMoment1(1.0 / eta);
    float cosTheta = fabs(dot(pi.hitPoint, wi));
    return (1 - fresnelReflFactor(cosTheta, eta)) / (c * PI);
}

float Separable::sampleSw(Shade &pi, dvec3 &wi, float *pdf) {
    dvec3 w = pi.normal, u, v;
    tangentSpace(w, u, v);
    dvec3 samplePt = smplrP->sampleUnitHemisphere();
    wi = normalize(samplePt.x * u + samplePt.y * v + samplePt.z * w);
    *pdf = INV_PI * dot(pi.normal, wi);
    return calcSw(pi, wi);
}

vec3 Separable::calcSp(Shade &po, Shade &pi) {
    return calcSr(distance(po.hitPoint, pi.hitPoint));
}

vec3 Separable::sampleS(Shade &po, double u1, const dvec2 &u2, Shade &pi, float *pdf)
{
    vec3 Sp = sampleSp(po, u1, u2, pi, pdf);
    if (Sp != Color::BLACK) {
        // Initialize material model at sampled surface interaction
    }
    return Sp;
}

vec3 Separable::sampleSp(Shade &po, double u1, const dvec2 &u2, Shade &pi, float *pdf)
{
    // Choose projection axis for BSSRDF sampling
    dvec3 vx, vy, vz;
    dvec3 ss, ts, ns = po.normal;
    tangentSpace(ns, ss, ts);
    if (u1 < 0.5f) {
        vx = ss, vy = ts, vz = ns;
        u1 *= 2;
    } else if (u1 < 0.75f) {
        vx = ts, vy = ns, vz = ss;
        u1 = (u1 - 0.5f) * 4;
    } else {
        vx = ns, vy = ss, vz = ts;
        u1 = (u1 - 0.75f) * 4;
    }

    // Choose spectral channel for BSSRDF sampling
    int channel = clamp(int(u1 * 3), 0, 2);
    u1 = 3 * u1 - channel;

    // Sample BSSRDF profile in polar coordinates
    double radius = sampleSr(channel, u2[0]);
    if (radius < 0) return Color::BLACK;
    double phi = 2 * PI * u2[1];

    // Compute BSSRDF profile bounds and intersection height
    double rMax = sampleSr(channel, 0.999f);
    if (radius >= rMax) return Color::BLACK;
    double length = 2 * sqrt(rMax * rMax - radius * radius);

    // Compute BSSRDF sampling ray segment
    Ray ray;
    ray.origin = po.hitPoint + radius * (vx * cos(phi) + vy * sin(phi)) - length * vz * 0.5;
    ray.direction = vz;

    // Intersect BSSRDF sampling ray against the scene geometry
    // Declare linked list
    struct IntersectionChain {
        Shade shade;
        IntersectionChain *next = nullptr;
        IntersectionChain(World &world) : shade(world) {}
        ~IntersectionChain() { if (next) delete next; }
    };

    auto chain = new IntersectionChain(po.world);
    auto ptr = chain, start = chain;
    int numFound = 0;

    while (true) {
        Shade shade = po.world.intersectObjects(ray);
        if (!shade.hasHit || shade.t > length) break;
        ray.origin = shade.hitPoint;
        ptr->shade = shade;

        // Append admissible intersection to chain
        if (ptr->shade.materialP == materialP) {
            auto next = new IntersectionChain(po.world);
            ptr->next = next;
            ptr = next;
            numFound++;
        }
    }
    // Randomly choose one of several intersections
    if (numFound == 0) return Color::BLACK;
    int selected = clamp(int(u1 * numFound), 0, numFound - 1);
    while (selected-- > 0) chain = chain->next;
    pi = chain->shade;
    delete start;

    // Compute sample PDF and return the spatial term
    // Express pi - po and ni with respect to local coordinate at po
    dvec3 dir = po.hitPoint - pi.hitPoint;
    dvec3 dLocal(dot(ss, dir), dot(ts, dir), dot(ns, dir));
    dvec3 nLocal(dot(ss, pi.normal), dot(ts, pi.normal), dot(ns, pi.normal));
    // Compute BSSRDF profile radius under projection along each axis
    dvec3 rProj = vec3(glm::length(dvec3(0, dLocal.y, dLocal.z)),
    glm::length(dvec3(dLocal.x, 0, dLocal.z)), glm::length(dvec3(dLocal.x, dLocal.y, 0)));
    // Return combined probability from all BSSRDF sampling strategies
    *pdf = 0;
    vec3 axisProb = vec3(0.25f, 0.25f, 0.5f);
    float chProb = 1.0f / 3.0f;
    for (int axis = 0; axis < 3; axis++)
        for (int ch = 0; ch < 3; ch++)
            *pdf += pdfSr(ch, rProj[axis]) * abs(nLocal[axis]) * chProb * axisProb[axis];

    return calcSp(po, pi);
}

Separable::~Separable() {
    delete smplrP;
}
