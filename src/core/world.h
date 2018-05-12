#pragma once

#include "utilities.h"
#include "core/viewplane.h"
#include "core/tracer.h"
#include "core/geometry.h"
#include "core/camera.h"
#include "core/light.h"
#include "photon/photonmap.h"

class World {
    friend class Camera;
public: 
    static int NUM_PHOTONS_PER_LIGHT;

    ViewPlane vp;
    Tracer *tracerP;
    Camera *cameraP;
    vec3 bgColor;
    vector<Geometry*> objects;
    Light *ambientP;
    vector<Light*> lights;
    PhotonMap *photonMap;

    World() : bgColor(0.0) {}
    void setup();
    Shade intersectObjects(Ray &ray);
    void render();
    void plotPoint(int row, int col, vec3 color);
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

    inline void setBackgroundColor(vec3 color) {
        bgColor = color;
    }

    inline void setCamera(Camera *camera_ptr) {
        cameraP = camera_ptr;
    }

  private:
    unsigned char *_pixels;
    atomic<unsigned long> finished;

    void displayStatus();
    void build();
};


