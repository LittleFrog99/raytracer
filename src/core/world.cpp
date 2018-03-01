#include "world.h"
#include "core/tracer.h"
#include "core/shade.h"
#include "tracer/multipleobjects.h"
#include "geometry/sphere.h"
#include "geometry/plane.h"
#include "geometry/function2d.h"
#include "sampler/regular.h"
#include "sampler/multijittered.h"
#include "utilities.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <limits.h>
#include <iostream>

void World::build() {
    vp.horRes = 300;
    vp.vertRes = 300;
    vp.pixelSize = 1;
    vp.numChannels = DEFAULT_NUM_CHANNELS;
    vp.setSampler(new MultiJittered(25, 2));
    vp.gamma = 1.0;
    bgColor = vec3(0.0f);

    auto *sphereP = new Sphere(dvec4(0.0, -25.0, 0.0, 1.0), 80.0, vec3(1.0f, 0.0f, 0.0f));
    addObject(sphereP);
    sphereP = new Sphere(dvec4(0.0, 30.0, 0.0, 1.0), 60, vec3(1, 1, 0));
    addObject(sphereP);
    auto *planeP = new Plane(dvec3(0.0, 1.0, 1.0), dvec4(0), vec3(0.0, 0.0, 1.0));
    addObject(planeP);

    tracerP = new MultipleObjects(this);
    _pixels = new unsigned char[vp.horRes * vp.vertRes * vp.numChannels];
}

void World::addObject(Geometry *obj) {
    objects.push_back(obj);
}

Shade World::intersectWithObjects(const Ray &ray) {
    Shade shade(*this);
    double t;
    double tmin = DBL_MAX;

    for (unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->intersect(ray, t, shade) && (t < tmin)) {
            shade.hasHit = true;
            tmin = t;
            shade.color = objects[i]->color;
        }
    }
    return shade;
}

void World::renderScene() {
    Random random;
    vec3 pixelColor;
    Ray ray;
    ray.direction = dvec4(0, 0, -1, 0);
    double zw = 100;

    for (int row = 0; row < vp.horRes; row++)
        for (int col = 0; col < vp.vertRes; col++) {
            pixelColor = vec3(0.0f);

            for (int p = 0; p < vp.numSamples; p++) { // regular(q + 0.5) / jittered sampling (q + rand)
                dvec2 sp = vp.getSampler()->sampleUnitSquare();
                double x = vp.pixelSize * (col - 0.5 * vp.horRes + sp.x);
                double y = vp.pixelSize * (row - 0.5 * vp.vertRes + sp.y);
                ray.origin = vec4(x, y, zw, 1);
                pixelColor += tracerP->traceRay(ray);
            }
            pixelColor /= vp.numSamples;
            plotPoint(row, col, vec4(pixelColor, 1.0));
        }
}

inline void World::plotPoint(unsigned int row, unsigned int col, vec4 color) {
    unsigned int offset = vp.numChannels * vp.horRes * row + vp.numChannels * col;
    _pixels[offset] = static_cast<unsigned char>(UCHAR_MAX * color.r); // red channel
    _pixels[offset + 1] = static_cast<unsigned char>(UCHAR_MAX * color.g); // green
    _pixels[offset + 2] = static_cast<unsigned char>(UCHAR_MAX * color.b); // blue
    _pixels[offset + 3] = static_cast<unsigned char>(UCHAR_MAX * color.a); // alpha
}

void World::output(string path) const {
    stbi__flip_vertically_on_write = 1;
    stbi_write_png(path.c_str(), vp.horRes, vp.vertRes, vp.numChannels, _pixels, 0);
}

World::~World() {
    delete tracerP;
    delete _pixels;
    for (unsigned int i = 0; i < objects.size(); i++)
        delete objects[i];
}
