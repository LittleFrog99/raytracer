#include "camera.h"
#include "core/world.h"

Camera::Camera(dvec3 eye, dvec3 look_at, dvec3 up, float exp_time) :
    eye(eye), lookAt(look_at), up(up), exposureTime(exp_time) {
    computeUVW();
}

void Camera::computeUVW() {
    w = normalize(eye - lookAt);
    u = normalize(cross(up, w));
    v = cross(w, u);
}
