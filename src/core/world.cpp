#include "world.h"
#include "core/tracer.h"
#include "core/shade.h"
#include "core/material.h"
#include "tracer/raycast.h"
#include "geometry/sphere.h"
#include "geometry/plane.h"
#include "geometry/function2d.h"
#include "sampler/regular.h"
#include "sampler/multijittered.h"
#include "camera/pinhole.h"
#include "camera/thinlens.h"
#include "light/ambient.h"
#include "light/pointlight.h"
#include "material/matte.h"
#include "utilities.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void World::build() {
    /* Viewplane */
    vp.horRes = 400;
    vp.vertRes = 400;
    vp.pixelSize = 0.5;
    vp.numChannels = DEFAULT_NUM_CHANNELS;
    vp.setSamples(25, 2);
    vp.gamma = 1.0;

    /* Lights */
    ambientP = new Ambient(vec3(1.0), 0.1);
    PointLight *lightP = new PointLight(dvec3(100, 100, 100), vec3(3.0), 0.8);
    lights.push_back(lightP);

    /* Materials */
    Matte *material1P = new Matte(vec3(1.0, 0.0, 0.0), 1.0, 1.0);
    Matte *material2P = new Matte(vec3(0.0, 1.0, 0.0), 1.0, 1.0);
    Matte *material3P = new Matte(vec3(0.0, 0.0, 1.0), 1.0, 1.0);

    /* Geometry Objects */
    auto *sphere1P = new Sphere(dvec3(-50.0, 0.0, 0.0), 80.0, material1P);
    auto *sphere2P = new Sphere(dvec3(40.0, 0.0, 0.0), 60, material2P);
    auto *plane1P = new Plane(dvec3(0, 1, 0), dvec3(0.0, -20.0, 0.0), material3P);
    //addObject(sphere1P);
    addObject(sphere1P);addObject(sphere2P);addObject(plane1P);

    /* Camera & Tracer */ 
    tracerP = new RayCast(this);
    PinHole *cam = new PinHole(dvec3(0, 50, 300), dvec3(-10, 0, 0), 150);
    cameraP = cam;
    _pixels = new unsigned char[vp.horRes * vp.vertRes * vp.numChannels];
}

void World::addObject(Geometry *obj) {
    objects.push_back(obj);
}

Shade World::hitObjects(Ray &ray) {
    Shade shade(*this);
    double t;
    double tmin = numeric_limits<double>::max();
    dvec3 normal, hitPoint, localHitPoint;

    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->intersect(ray, t, shade) && (t < tmin)) {
            shade.hasHit = true;
            tmin = t;
            shade.materialP = objects[i]->materialP;
            shade.hitPoint = ray.origin + t * ray.direction;
            normal = shade.normal;
            hitPoint = shade.hitPoint;
            localHitPoint = shade.localHitPoint;
        }
    }

    if (shade.hasHit) { // only the nearest object
        shade.t = tmin;
        shade.normal = normal;
        shade.hitPoint = hitPoint;
        shade.localHitPoint = localHitPoint;
    }

    return shade;
}

void World::renderScene() {
    cameraP->renderScene(*this);
}

void World::plotPoint(int row, int col, vec4 color) {
    static int MAX = numeric_limits<unsigned char>::max();
    unsigned int offset = vp.numChannels * vp.horRes * row + vp.numChannels * col;
    _pixels[offset] = static_cast<unsigned char>(MAX * color.r);             // red
    _pixels[offset + 1] = static_cast<unsigned char>(MAX * color.g);         // green
    _pixels[offset + 2] = static_cast<unsigned char>(MAX * color.b);         // blue
    _pixels[offset + 3] = static_cast<unsigned char>(MAX * color.a);         // alpha
}

void World::output(string path) {
    stbi__flip_vertically_on_write = 1;
    stbi_write_png(path.c_str(), vp.horRes, vp.vertRes, vp.numChannels, _pixels, 0);
}

World::~World() {
    delete tracerP;
    delete cameraP;
    delete ambientP;
    delete _pixels;
    for (int i = 0; i < objects.size(); i++)
        delete objects[i];
    for (int i = 0; i < lights.size(); i++)
        delete lights[i];
}
