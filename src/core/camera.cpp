#include "camera.h"
#include "core/world.h"

int Camera::RENDER_THREAD = 1;

Camera::Camera(dvec3 eye, dvec3 look_at, dvec3 up, float exp_time) :
    eye(eye), lookAt(look_at), up(up), exposureTime(exp_time) {
    computeUVW();
}

void Camera::computeUVW() {
    w = normalize(eye - lookAt);
    u = normalize(cross(up, w));
    v = cross(w, u);

    if (eye.x == lookAt.x && eye.z == lookAt.z) {  // camera looking vertically
        if (eye.y > lookAt.y)
            u = dvec3(0, 0, 1), v = dvec3(1, 0, 0), w = dvec3(0, 1, 0); // down
        else 
            u = dvec3(1, 0, 0), v = dvec3(0, 0, 1), w = dvec3(0, -1, 0); // up
    }
}

void Camera::setRollAngles(double radians) {
    dmat4 rotMat = rotate(dmat4(), -radians, w);
    u = dmat3(rotMat) * u;
    v = dmat3(rotMat) * v;
}

void Camera::renderThread(int index, int step) {
    while (index < vp.horRes * vp.vertRes) {
        ivec2 coord = vp.toPixelCoord(index);
        renderPixel(coord.x, coord.y);
        index += step;
    }
};

void Camera::render(World &w) {
    world = &w;
    vp = w.vp;
    vp.pixelSize /= zoom;

    thread render[RENDER_THREAD];
    for (int i = 0; i < RENDER_THREAD; i++) {
        render[i] = thread(&Camera::renderThread, this, i, RENDER_THREAD);
        render[i].join();
    }
}