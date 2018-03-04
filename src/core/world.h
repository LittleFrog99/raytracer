#pragma once

#include <vector>
#include <string>
#include "core/viewplane.h"
#include "core/tracer.h"
#include "core/geometry.h"
#include "core/camera.h"
using namespace std;

class World {
public:
    ViewPlane vp;
    vec3 bgColor;
    Tracer *tracerP;
    Camera *cameraP;
    vector<Geometry *> objects;

    World() {}
    ~World();
    void build();
    void addObject(Geometry *obj);
    Shade intersectWithObjects(const Ray &ray);
    void renderScene();
    void plotPoint(int row, int col, vec4 color);
    void output(string path) const;

private:
    unsigned char *_pixels;
    constexpr static int DEFAULT_NUM_CHANNELS = 4;
};


