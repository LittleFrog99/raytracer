#include "world.h"
#include "core/tracer.h"
#include "core/shade.h"
#include "core/material.h"
#include "tracer/arealighttracer.h"
#include "geometry/sphere.h"
#include "geometry/plane.h"
#include "geometry/rectangle.h"
#include "geometry/box.h"
#include "geometry/triangle.h"
#include "geometry/disk.h"
#include "sampler/regular.h"
#include "sampler/multijittered.h"
#include "camera/pinhole.h"
#include "camera/thinlens.h"
#include "light/ambientoccluder.h"
#include "light/pointlight.h"
#include "light/arealight.h"
#include "material/matte.h"
#include "material/phong.h"
#include "material/emissive.h"
#include "utilities.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void World::build() {
    /* Viewplane */
    vp.horRes = 400;
    vp.vertRes = 400;
    vp.pixelSize = 1;
    vp.numChannels = DEFAULT_NUM_CHANNELS;
    vp.setSamples(16, 2);
    vp.gamma = 1.0;

    /* Materials */
    auto *material1P = new Phong(vec3(0.25, 0.72, 0.96), 0.4, 0.4, 0.1);
    auto *material2P = new Phong(vec3(0.44, 0.24, 0.61), 0.4, 0.6, 0.1);
    material2P->setSpecularExponent(8.0f);
    auto *material3P = new Matte(vec3(1.0), 0.5, 1.0);
    auto *material4P = new Emissive(vec3(1.0), 100.0);
    auto *material5P = new Phong(vec3(0.14, 0.47, 0.8), 0.3, 0.6, 0.1);
    auto *material6P = new Phong(vec3(0.89, 0.36, 0.14), 0.3, 0.6, 0.15);

    /* Geometry Objects */
    auto *sphere1P = new Sphere(material1P);
    sphere1P->setParams(dvec3(-30.0, 80.0, -20.0), 80.0);
    auto *sphere2P = new Sphere(material2P);
    sphere2P->setParams(dvec3(60.0, 45.0, 80.0), 45.0);
    auto *plane1P = new Plane(material3P);
    plane1P->setParams(dvec3(0, 1, 0), dvec3(0.0, 0.0, 0.0));
    auto *rect1P = new Rectangle(material4P);
    rect1P->setParams(dvec3(-30, 300, 200), dvec3(60, 0, 0), dvec3(0, -40, 20));
    rect1P->setSampler(new MultiJittered(256, 2));
    rect1P->toggleShadowCast(false);
    auto *box1P = new Box(material5P);
    box1P->setParams(dvec3(50, 0, -120), dvec3(180, 200, -50));
    auto *triangle1P = new Triangle(material6P);
    triangle1P->setParams(dvec3(-30, 40, 140), dvec3(30, 15, 180), dvec3(90, 60, 130));
    auto *disk1P = new Disk(material6P);
    disk1P->setParams(dvec3(20, 40, 50), dvec3(0, 10, 5), 30);

    addObject(sphere1P);
    addObject(sphere2P);
    addObject(plane1P);
    addObject(box1P);
    addObject(triangle1P);
    addObject(disk1P);

    /* Lights */
    bgColor = vec3(0.41, 0.72, 0.83);
    AmbientOccluder *occluderP = new AmbientOccluder(vec3(0.41, 0.72, 0.83), 1.0, 0.2);
    occluderP->setSampler(new MultiJittered(256, 2));
    occluderP->setOcclusionSamples(1);
    setAmbient(occluderP);

    auto area1P = new AreaLight(rect1P);
    area1P->toggleShadowCast(true);
    addLight(area1P);

    /* Camera & Tracer */ 
    tracerP = new AreaLightTracer(this);
    PinHole *cam = new PinHole(dvec3(-100, 150, 240), dvec3(0, 30, 0), 200);
    cameraP = cam;
    _pixels = new unsigned char[vp.horRes * vp.vertRes * vp.numChannels];
    finished = 0;
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

void World::displayStatus() {
    int total = vp.horRes * vp.vertRes;
    while (finished <= total) {
        cout << '\r';
        cout << setw(8) << setprecision(4) << float(finished) * 100 / total << '%';
        cout.flush();
        if (finished == total)
            break;
        usleep(100000);
    }
}

void World::renderScene() {
    thread status(&World::displayStatus, this);
    status.detach();
    cameraP->renderScene(*this);
    usleep(100000);
}

vec3 maxToOne(vec3 color) {
    float maxValue = Math::maxComponent(color);
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

    finished++;
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
