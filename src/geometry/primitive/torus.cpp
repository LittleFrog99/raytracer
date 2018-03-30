#include "torus.h"

void Torus::setParams(double swept_r, double tube_r) {
    sweptR = swept_r;
    tubeR = tube_r;
    bndBox = BoundingBox(dvec3(-sweptR - tubeR, -tubeR, -sweptR - tubeR),
                         dvec3(sweptR + tubeR, tubeR, sweptR + tubeR));
}

bool Torus::intersect(Ray &ray, double &tmin, Shade &shade) {
    if (!bndBox.intersect(ray)) return false;

    double dSqrd = Math::lengthSquared(ray.direction);
    double e = Math::lengthSquared(ray.origin) - sweptR * sweptR - tubeR * tubeR;
    double f = dot(ray.origin, ray.direction);
    double fourASqrd = 4.0 * sweptR * sweptR;

    // Determine coefficients: c0 + c1 * t + c2 * t^2 + c3 * t^3 + c4 * t^4 = 0
    double coeffs[5], roots[4];
    coeffs[0] = e * e - fourASqrd * (tubeR * tubeR - pow(ray.origin.y, 2));
    coeffs[1] = 4.0 * f * e + 2.0 * fourASqrd * ray.origin.y * ray.direction.y;
    coeffs[2] = 2.0 * dSqrd * e + 4.0 * f * f + fourASqrd * pow(ray.direction.y, 2);
    coeffs[3] = 4.0 * dSqrd * f;
    coeffs[4] = dSqrd * dSqrd;

    int numRealRoots = Math::solveQuartic(coeffs, roots);
    bool hit = false;
    double t = numeric_limits<double>::max();

    if (numRealRoots == 0) return false;
    for (int i = 0; i < numRealRoots; i++) 
        if (roots[i] > EPSILON) {
            hit = true;
            if (roots[i] < t) t = roots[i];
        }
    if (!hit) return false;

    tmin = t;
    shade.localHitPoint = ray.origin + t * ray.direction;
    shade.hitPoint = shade.localHitPoint;
    shade.normal = getNormal(shade.localHitPoint);
    return true;
}

bool Torus::shadowIntersect(Ray &ray, double &tmin) {
    if (!bndBox.intersect(ray)) return false;

    double dSqrd = Math::lengthSquared(ray.direction);
    double e = Math::lengthSquared(ray.origin) - sweptR * sweptR - tubeR * tubeR;
    double f = dot(ray.origin, ray.direction);
    double fourASqrd = 4.0 * sweptR * sweptR;

    double coeffs[5], roots[4];
    coeffs[0] = e * e - fourASqrd * (tubeR * tubeR - pow(ray.origin.y, 2));
    coeffs[1] = 4.0 * f * e + 2.0 * fourASqrd * ray.origin.y * ray.direction.y;
    coeffs[2] = 2.0 * dSqrd * e + 4.0 * f * f + fourASqrd * pow(ray.direction.y, 2);
    coeffs[3] = 4.0 * dSqrd * f;
    coeffs[4] = dSqrd * dSqrd;

    int numRealRoots = Math::solveQuartic(coeffs, roots);
    bool hit = false;
    double t = numeric_limits<double>::max();

    if (numRealRoots == 0) return false;
    for (int i = 0; i < numRealRoots; i++) 
        if (roots[i] > EPSILON) {
            hit = true;
            if (roots[i] < t) t = roots[i];
        }
    if (!hit) return false;

    tmin = t;
    return true;
}

dvec3 Torus::getNormal(dvec3 &point) {
    double ptSqrd = Math::lengthSquared(point);
    double rSqrd = sweptR * sweptR + tubeR * tubeR;
    return normalize(dvec3(point.x * (ptSqrd - rSqrd),
                           point.y * (ptSqrd - rSqrd + 2 * sweptR * sweptR),
                           point.z * (ptSqrd - rSqrd)));
}