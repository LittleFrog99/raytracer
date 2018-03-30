#include "world.h"
#include "geometry/sphere.h"
#include "geometry/plane.h"
#include "geometry/rectangle.h"
#include "geometry/box.h"
#include "geometry/triangle.h"
#include "geometry/disk.h"
#include "geometry/opencylinder.h"
#include "geometry/torus.h"
#include "material/matte.h"
#include "material/phong.h"
#include "material/emissive.h"
#include "light/ambientoccluder.h"
#include "light/pointlight.h"
#include "light/arealight.h"
#include "tracer/arealighttracer.h"
#include "camera/pinhole.h"
#include "camera/thinlens.h"
#include "sampler/regular.h"
#include "sampler/multijittered.h"

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
    auto *material3P = new Matte(vec3(1.0), 0.5, 0.7);
    auto *material4P = new Emissive(vec3(1.0), 1000.0);
    auto *material5P = new Phong(vec3(0.14, 0.47, 0.8), 0.3, 0.6, 0.1);
    auto *material6P = new Phong(vec3(0.89, 0.36, 0.14), 0.3, 0.6, 0.15);

    /* Geometry Objects */
    auto *sphere1P = new Sphere(material1P);
    sphere1P->setParams(dvec3(-100, 80, -50), 80.0);
    auto *sphere2P = new Sphere(material2P);
    sphere2P->setParams(dvec3(80.0, 45.0, 120.0), 45.0);
    auto *plane1P = new Plane(material3P);
    plane1P->setParams(dvec3(0, 1, 0), dvec3(0.0, -30.0, 0.0));
    auto *box1P = new Box(material5P);
    box1P->setParams(dvec3(30, 0, -30), dvec3(160, 120, 40));
    auto *triangle1P = new Triangle(material6P);
    triangle1P->setParams(dvec3(-80, 40, 140), dvec3(-20, 15, 180), dvec3(40, 60, 130));
    auto *disk1P = new Disk(material4P);
    disk1P->setParams(dvec3(0, 200, 120), dvec3(0, -1, 0), 30);
    disk1P->setSampler(new MultiJittered(256, 2));
    disk1P->toggleShadowCast(false);
    auto torusP = new Torus(material6P);
    torusP->setParams(80, 20);

    addObject(sphere1P);
    addObject(sphere2P);
    addObject(plane1P);
    addObject(box1P);
    addObject(triangle1P);
    addObject(disk1P);
    addObject(torusP);

    /* Lights */
    bgColor = vec3(0.41, 0.72, 0.83);
    AmbientOccluder *occluderP = new AmbientOccluder(vec3(0.41, 0.72, 0.83), 1.0, 0.2);
    occluderP->setSampler(new MultiJittered(256, 2));
    occluderP->setOcclusionSamples(1);
    setAmbient(occluderP);

    auto area1P = new AreaLight(disk1P);
    area1P->toggleShadowCast(true);
    addLight(area1P);

    /* Camera & Tracer */
    tracerP = new AreaLightTracer(this);
    PinHole *cam = new PinHole(dvec3(0, 150, 300), dvec3(0, 50, 0), 200);
    cameraP = cam;
    _pixels = new unsigned char[vp.horRes * vp.vertRes * vp.numChannels];
    finished = 0;
}