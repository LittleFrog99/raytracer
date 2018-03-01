#pragma once

#include <vector>
#include <string>
#include "core/viewplane.h"
#include "core/tracer.h"
#include "core/geometry.h"
using namespace std;

class World {
public:
    ViewPlane vp;
    vec3 bgColor;
    Tracer *tracerP;
    vector<Geometry *> objects;

    World() {}
    ~World();
    void build();
    void addObject(Geometry *obj);
    Shade intersectWithObjects(const Ray &ray);
    void renderScene();
    void output(std::string path) const;

private:
    unsigned char *_pixels;
    constexpr static int DEFAULT_NUM_CHANNELS = 4;

    void plotPoint(unsigned int row, unsigned int col, vec4 color);
};


