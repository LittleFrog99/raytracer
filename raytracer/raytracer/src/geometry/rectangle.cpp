#include "rectangle.h"
#include "core/sampler.h"

void Rectangle::setParams(dvec3 origin, dvec3 side_a, dvec3 side_b) {
    this->origin = origin;
    sideA = side_a;
    sideB = side_b;
    normal = normalize(cross(sideA, sideB));
    invArea = 1 / (length(sideA) * length(sideB));
}

dvec3 Rectangle::sample() {
    dvec2 samplePt = samplerP->sampleUnitSquare();
    return (origin + samplePt.x * sideA + samplePt.y * sideB);
}

float Rectangle::probDensity(Shade &shade) {
    return invArea;
}

dvec3 Rectangle::getNormal(dvec3 &point) {
    return normal;
}