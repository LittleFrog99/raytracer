#pragma once
#include "utilities.h"

class World;
class Camera {
public:
    Camera(dvec3 eye, dvec3 look_at, dvec3 up = dvec3(0, 1, 0), float exp_time = 1.0);
    inline void setExposureTime(float time) { exposureTime = time; }
    void setRollAngles(double radians);
    virtual void renderScene(World &w) = 0;
    virtual ~Camera() {}

protected:
    dvec3 eye, lookAt, up;
    dvec3 u, v, w;
    float exposureTime;

    void computeUVW();
};
