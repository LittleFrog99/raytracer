#include "world.h"
#include "core/tracer.h"
#include "core/shade.h"
#include "core/material.h"
#include "tracer/arealighttracer.h"
#include "geometry/sphere.h"
#include "geometry/plane.h"
#include "geometry/rectangle.h"
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
    vp.pixelSize = 0.5;
    vp.numChannels = DEFAULT_NUM_CHANNELS;
    vp.setSamples(25, 2);
    vp.gamma = 1.0;

    /* Materials */
    auto *material1P = new Matte(vec3(1.0, 0, 0), 0.2, 0.4);
    auto *material2P = new Matte(vec3(0.0, 1.0, 0), 0.2, 0.4);
    auto *material3P = new Matte(vec3(0.6), 0, 3.0);
    auto *material4P = new Emissive(vec3(1.0), 100.0);

    /* Geometry Objects */
    auto *sphere1P = new Sphere(material1P);
    sphere1P->setParams(dvec3(-30.0, 80.0, -20.0), 80.0);
    auto *sphere2P = new Sphere(material2P);
    sphere2P->setParams(dvec3(40.0, 40.0, 80.0), 40.0);
    auto *plane1P = new Plane(material3P);
    plane1P->setParams(dvec3(0, 1, 0), dvec3(0.0, 0.0, 0.0));
    auto *rect1P = new Rectangle(material4P);
    rect1P->setParams(dvec3(-50, 300, 200), dvec3(50, 0, 0), dvec3(0, -50, 0));
    rect1P->setSampler(new MultiJittered(100, 2));
    rect1P->toggleShadowCast(false);

    addObject(sphere1P);
    addObject(sphere2P);
    addObject(plane1P);
    // addObject(rect1P);

    /* Lights */
    bgColor = vec3(0.8);
    AmbientOccluder *occluderP = new AmbientOccluder(vec3(1.0), 1.0, 0.2);
    occluderP->setSampler(new MultiJittered(256, 2));
    occluderP->setOcclusionSamples(1);
    setAmbient(occluderP);

    auto area1P = new AreaLight(rect1P);
    area1P->toggleShadowCast(true);
    addLight(area1P);

    /* Camera & Tracer */ 
    tracerP = new AreaLightTracer(this);
    PinHole *cam = new PinHole(dvec3(0, 300, 0), dvec3(0, 30, 0), 100);
    cameraP = cam;
    _pixels = new unsigned char[vp.horRes * vp.vertRes * vp.numChannels];
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

void displayStatus(World *world) {
    int total = world->vp.horRes * world->vp.vertRes;
    while (world->finished <= total) {
        cout << '\r';
        cout << setw(8) << setprecision(4) << float(world->finished) * 100 / total << '%';
        cout.flush();
        if (world->finished == total)
            break;
        usleep(100000);
    }
}

void World::renderScene() {
    thread status(displayStatus, this);
    status.detach();
    cameraP->renderScene(*this);
    usleep(100000);
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
