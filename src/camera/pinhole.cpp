#include "pinhole.h"
#include "core/world.h"

void PinHole::renderScene(World &w) {
    vec3 color;
    ViewPlane vp(w.vp);
    Ray ray;
    dvec2 samplePt, pixelPt;

    vp.pixelSize /= zoom;
    ray.origin = eye;

    for (int row = 0; row < vp.vertRes; row++)
        for (int col = 0; col < vp.horRes; col++) {
            color = vec3(0.0);

            for (int i = 0; i < vp.numSamples; i++) {
                samplePt = vp.samplerP->sampleUnitSquare();
                pixelPt = dvec2(vp.pixelSize * (col - 0.5 * vp.horRes + samplePt.x),
                                vp.pixelSize * (row - 0.5 * vp.vertRes + samplePt.y));
                ray.direction = rayDirection(pixelPt);
                color += w.tracerP->traceRay(ray);
            }

            color /= vp.numSamples;
            color *= exposureTime;
            w.plotPoint(row, col, vec4(color, 1.0));
        }
}

inline dvec3 PinHole::rayDirection(const dvec2 &pt) const {
    return normalize(pt.x * u + pt.y * v - distance * w);
}
