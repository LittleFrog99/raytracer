#pragma once
#include "core/camera.h"

class PinHole : public Camera {
public:
    PinHole() {}
    PinHole(dvec3 eye, dvec3 look_at, double dist, dvec3 up = dvec3(0, 1, 0)) :
        Camera(eye, look_at, up), distance(dist) {}
    virtual void renderPixel(int row, int col);

private:
    double distance;

    dvec3 rayDirection(const dvec2 &pt) const;
};
