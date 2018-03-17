#include "world.h"
#include "core/tracer.h"
#include "core/shade.h"
#include "core/material.h"
#include "tracer/raycast.h"
#include "geometry/sphere.h"
#include "geometry/plane.h"
#include "sampler/regular.h"
#include "sampler/multijittered.h"
#include "camera/pinhole.h"
#include "camera/thinlens.h"
#include "light/ambient.h"
#include "light/pointlight.h"
#include "material/matte.h"
#include "material/phong.h"
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
    ambientP = new Ambient(vec3(1.0), 0.05);
    PointLight *light1P = new PointLight(dvec3(150, 100, 200), vec3(2.2), 1.0);
    PointLight *light2P = new PointLight(dvec3(-120, 100, 100), vec3(1.7), 1.0);
    lights.push_back(light1P); lights.push_back(light2P);

    /* Materials */
    Phong *material1P = new Phong(vec3(1.0, 0.0, 0.0), 1.0, 0.75, 0.20);
    material1P->setSpecularExponent(16.0f);
    Phong *material2P = new Phong(vec3(0.0, 1.0, 0.0), 1.0, 0.7, 0.25);
    material2P->setSpecularExponent(12.0f);
    Phong *material3P = new Phong(vec3(0.0, 0.0, 1.0), 1.0, 0.9, 0.1);

    /* Geometry Objects */
    auto *sphere1P = new Sphere(dvec3(-20.0, 0.0, 0.0), 80.0, material1P);
    auto *sphere2P = new Sphere(dvec3(80.0, 0.0, -20.0), 60, material2P);
    auto *plane1P = new Plane(dvec3(0, 1, 0), dvec3(0.0, -40.0, 0.0), material3P);
    addObject(sphere1P);addObject(sphere2P);addObject(plane1P);

    /* Camera & Tracer */ 
    tracerP = new RayCast(this);
    PinHole *cam = new PinHole(dvec3(0, 50, 300), dvec3(10, 0, 0), 150);
    cameraP = cam;
    _pixels = new unsigned char[vp.horRes * vp.vertRes * vp.numChannels];
}

void World::addObject(Geometry *obj) {
    objects.push_back(obj);
}

Shade World::intersectObjects(Ray &ray) {
    Shade shade(*this);
    double t;
    double tmin = numeric_limits<double>::max();
    dvec3 normal, hitPoint, localHitPoint;

    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->intersect(ray, t, shade) && (t < tmin)) {
            shade.hasHit = true;
            tmin = t;
            shade.materialP = objects[i]->getMaterial();
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

vec3 maxToOne(vec3 color) {
    float maxValue = glm::max(glm::max(color.r, color.g), color.b);
    return maxValue > 1 ? (color / maxValue) : color;
}

void World::plotPoint(int row, int col, vec4 color) {
    color = vec4(maxToOne(vec3(color)), color.a);

    static float MAX = numeric_limits<unsigned char>::max();
    unsigned int offset = vp.numChannels * vp.horRes * row + vp.numChannels * col;
    _pixels[offset] = static_cast<unsigned char>(MAX * color.r);
    _pixels[offset + 1] = static_cast<unsigned char>(MAX * color.g);
    _pixels[offset + 2] = static_cast<unsigned char>(MAX * color.b);
    _pixels[offset + 3] = static_cast<unsigned char>(MAX * color.a);
}

void World::output(string path) {
    stbi__flip_vertically_on_write = 1;
    stbi_write_png(path.c_str(), vp.horRes, vp.vertRes, vp.numChannels, _pixels, 0);
}

World::~World() {
    delete tracerP;
    delete cameraP;
    delete ambientP;
    for (int i = 0; i < objects.size(); i++)
        delete objects[i];
    for (int i = 0; i < lights.size(); i++)
        delete lights[i];
    delete _pixels;
}
