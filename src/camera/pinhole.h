#pragma once
#include "core/camera.h"

class PinHole : public Camera {
public:
    PinHole(dvec3 eye, dvec3 look_at, double dist, dvec3 up = dvec3(0, 1, 0)) :
        Camera(eye, look_at, up), distance(dist) {}
    inline void setZoomFactor(double zm) { zoom = zm; }
    void renderScene(World &w);

private:
    double distance;
    double zoom = 1;

    dvec3 rayDirection(const dvec2 &pt) const;
};
