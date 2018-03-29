#pragma once

#include "utilities.h"
#include "core/viewplane.h"

class World;
class Camera {
public:
    static int RENDER_THREAD;

    Camera() {}
    Camera(dvec3 eye, dvec3 look_at, dvec3 up = dvec3(0, 1, 0), float exp_time = 1.0);
    inline void setExposureTime(float time) { exposureTime = time; }
    void setRollAngles(double radians);
    virtual void render(World &w);
    virtual void renderPixel(int row, int col) = 0;
    virtual ~Camera() {}

    inline void setZoomFactor(double zm) { zoom = zm; }

protected:
    dvec3 eye, lookAt, up;
    dvec3 u, v, w;
    World *world = nullptr;
    ViewPlane vp;
    double zoom = 1;
    float exposureTime;

    void computeUVW();
    void renderThread(int index, int step);
};
