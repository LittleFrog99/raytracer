#include "geometry.h"

void Geometry::setSampler(Sampler *sampler_ptr) {
    if (samplerP)
        delete samplerP;
    samplerP = sampler_ptr;
}

BoundingBox Geometry::getBoundingBox() {
    if (dynamic_cast<BoxBounded *>(this))
        return dynamic_cast<BoxBounded *>(this)->getBoundingBox();
    else return calcBoundingBox();
}