#include "thinlens.h"
#include "core/world.h"

void ThinLens::setSampler(Sampler *smpl) {
    if (samplerP) {
        delete samplerP;
        samplerP = nullptr;
    }
    samplerP = smpl;
    samplerP->mapSamplesToUnitDisk();
}

void ThinLens::setParams(double radius, double view_dist, double focal_dist, double zoomf) {
    lensRadius = radius;
    viewDist = view_dist;
    focalDist = focal_dist;
    zoom = zoomf;
}

dvec3 ThinLens::rayDirection(dvec2 &pix_pt, dvec2 &lens_pt) {
    dvec2 focPt = pix_pt * focalDist / viewDist;
    dvec3 dir = (focPt.x - lens_pt.x) * u + (focPt.y - lens_pt.y) * v - focalDist * w;
    return normalize(dir);
}

void ThinLens::render(World &world) {
    Ray ray;
    vp = world.vp;
    dvec2 smplPt, pixPt, lensPt;
    vp.pixelSize /= zoom;

    for (int row = 0; row < vp.horRes; row++) 
        for (int col = 0; col < vp.vertRes; col++) {
            vec3 color(0.0f);

            for (int i = 0; i < vp.numSamples; i++) {
                smplPt = vp.samplerP->sampleUnitSquare();
                pixPt = vp.pixelSize * dvec2(col - vp.horRes / 2.0 + smplPt.x, 
                        row - vp.vertRes / 2.0 + smplPt.y);
                
                lensPt = samplerP->sampleUnitDisk() * lensRadius;

                ray.origin = eye + lensPt.x * u + lensPt.y * v;
                ray.direction = rayDirection(pixPt, lensPt);
                color += world.tracerP->traceRay(ray);
            }

            color /= vp.numSamples;
            color *= exposureTime;
            world.plotPoint(row, col, vec4(color, 1.0));
        }
}
