#include "pinhole.h"
#include "core/world.h"

void PinHole::renderPixel(int row, int col) {
    vec3 color;
    dvec2 samplePt, pixelPt;
    Ray ray;
    ray.origin = eye;

    for (int i = 0; i < vp.numSamples; i++) {
        samplePt = vp.samplerP->sampleUnitSquare();
        pixelPt = dvec2(vp.pixelSize * (col - 0.5 * vp.horRes + samplePt.x),
                        vp.pixelSize * (row - 0.5 * vp.vertRes + samplePt.y));
        ray.direction = rayDirection(pixelPt);
        color += world->tracerP->traceRay(ray);
    }

    color /= vp.numSamples;
    color *= exposureTime;
    world->plotPoint(row, col, vec4(color, 1.0));
}

dvec3 PinHole::rayDirection(const dvec2 &pt) const {
    return normalize(pt.x * u + pt.y * v - distance * w);
}
