#include "parttorus.h"

void PartTorus::setParams(double swept_r, double tube_r, double azim_min, double azim_max,
                          double polar_min, double polar_max) {
    Torus::setParams(swept_r, tube_r);
    this->azimMin = azim_min;
    this->azimMax = azim_max;
    this->polarMin = polar_min;
    this->polarMax = polar_max;
}

bool PartTorus::inRange(dvec3 &point) {
    dvec3 proj = dvec3(point.x, 0, point.z);
    dvec3 circPt = normalize(proj) * sweptR;
    double azim = atan2(circPt.x, circPt.z);
    if (azim < 0) azim += 2 * PI;
    double polar = atan2(point.y, length(proj) - length(circPt));
    if (polar < 0) polar += 2 * PI;
    return azim < azimMax && azim > azimMin && polar < polarMax && polar > polarMin;
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
        if (roots[i] > EPSILON && roots[i] < t) {
            dvec3 hitPt = ray.origin + roots[i] * ray.direction;
            if (inRange(hitPt))  {
                t = roots[i];
                hit = true;
            }
        }

    if (hit) {
        tmin = t;
        shade.localHitPoint = ray.origin + t * ray.direction;
        shade.materialP = getMaterial();
        shade.normal = getNormal(shade.localHitPoint);
        if (dot(-ray.direction, shade.normal) < 0.0)
            shade.normal = -shade.normal;
    }
    return hit;
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
        if (roots[i] > EPSILON && roots[i] < t) {
            dvec3 hitPt = ray.origin + roots[i] * ray.direction;
            if (inRange(hitPt))  {
                t = roots[i];
                hit = true;
            }
        }

    if (hit)
        tmin = t;
    return hit;
}