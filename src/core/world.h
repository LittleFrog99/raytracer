#pragma once

#include "utilities.h"
#include "core/viewplane.h"
#include "core/tracer.h"
#include "core/geometry.h"
#include "core/camera.h"
#include "core/light.h"

class World {
    friend class Camera;
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
    Shade intersectObjects(Ray &ray);
    void renderScene();
    void plotPoint(int row, int col, vec4 color);
    void output(string path);
    ~World();

    inline void addObject(Geometry *obj_ptr) {
        objects.push_back(obj_ptr);
    }
    inline void addLight(Light *light_ptr) {
        lights.push_back(light_ptr);
    }
    inline void setAmbient(Light *ambient_ptr) {
        ambientP = ambient_ptr;
    }

  private:
    unsigned char *_pixels;
    atomic<int> finished;
    constexpr static int DEFAULT_NUM_CHANNELS = 4;

    void displayStatus();
};


