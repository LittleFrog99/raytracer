#pragma once

#include "utilities.h"
#include "core/viewplane.h"
#include "core/tracer.h"
#include "core/geometry.h"
#include "core/camera.h"
#include "core/light.h"

class World {
public:
    ViewPlane vp;
    Tracer *tracerP;
    Camera *cameraP;
    vec3 bgColor;
    vector<Geometry*> objects;
    Light *ambientP;
    vector<Light*> lights;

    World() : bgColor(0.0) {}
    void build();
    void addObject(Geometry *obj);
    Shade intersectObjects(Ray &ray);
    void renderScene();
    void plotPoint(int row, int col, vec4 color);
    void output(string path);
    ~World();

private:
    unsigned char *_pixels;
    constexpr static int DEFAULT_NUM_CHANNELS = 4;
};


