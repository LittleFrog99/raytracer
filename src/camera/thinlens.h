#pragma once

#include "core/camera.h"
#include "core/sampler.h"

class ThinLens : public Camera {
public:
    ThinLens(dvec3 eye, dvec3 look_at, dvec3 up = dvec3(0, 1, 0)) 
    : Camera(eye, look_at, up) {}
    void setSampler(Sampler *smpl);
    void setParams(double radius, double view_dist, double focal_dist, double zoomf = 1);
    void renderScene(World &w);

private:
    double lensRadius;
    double viewDist;
    double focalDist;
    double zoom = 1;
    Sampler *samplerP = nullptr;

    dvec3 rayDirection(dvec2 &pix_pt, dvec2 &lens_pt);
};
