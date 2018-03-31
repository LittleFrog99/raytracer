#include "parttorus.h"

void PartTorus::setParams(double swept_r, double tube_r, double phi_min, double phi_max,
                          double theta_min, double theta_max) {
    Torus::setParams(swept_r, tube_r);
    this->phiMax = phi_max;
    this->phiMin = phi_min;
    this->thetaMax = theta_max;
    this->thetaMin = theta_min;
}

bool PartTorus::inRange(dvec3 &point) {
    dvec3 proj = dvec3(point.x, 0, point.z);
    dvec3 circPt = normalize(proj) * sweptR;
    double phi = atan2(circPt.x, circPt.z);
    if (phi < 0) phi += 2 * PI;
    double theta = atan2(point.y, length(proj) - length(circPt));
    if (theta < 0) theta += 2 * PI;
    return phi < phiMax && phi > phiMin && theta < thetaMax && theta > thetaMin;
}

bool PartTorus::intersect(Ray &ray, double &tmin, Shade &shade) {
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

    dvec3 hitPt = ray.origin + t * ray.direction;
    if (inRange(hitPt)) {
        tmin = t;
        shade.localHitPoint = hitPt;
        shade.hitPoint = shade.localHitPoint;
        shade.normal = getNormal(shade.localHitPoint);
        if (dot(-ray.direction, shade.normal) < 0.0)
            shade.normal = -shade.normal;
        return true;
    }
    else return false;
}

bool PartTorus::shadowIntersect(Ray &ray, double &tmin) {
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
            dvec3 hitPt = ray.origin + roots[i] * ray.direction;
            if (inRange(hitPt)) {
                hit = true;
                break;
            }
        }
    return hit;
}