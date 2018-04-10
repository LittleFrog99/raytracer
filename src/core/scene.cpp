#include "world.h"
#include "geometry/primitive/sphere.h"
#include "geometry/primitive/plane.h"
#include "geometry/primitive/box.h"
#include "geometry/primitive/triangle.h"
#include "geometry/primitive/disk.h"
#include "geometry/primitive/rectangle.h"
#include "geometry/instance.h"
#include "geometry/compound/grid.h"
#include "geometry/model/model.h"
#include "material/matte.h"
#include "material/phong.h"
#include "material/emissive.h"
#include "material/reflective.h"
#include "material/glossyreflector.h"
#include "light/ambientoccluder.h"
#include "light/pointlight.h"
#include "light/arealight.h"
#include "tracer/globaltracer.h"
#include "camera/pinhole.h"
#include "camera/thinlens.h"
#include "sampler/regular.h"
#include "sampler/multijittered.h"

void World::build() {
    /* Viewplane */
    vp.horRes = 400;
    vp.vertRes = 400;
    vp.pixelSize = 0.5;
    vp.numChannels = DEFAULT_NUM_CHANNELS;
    vp.maxDepth = 5;
    vp.globalIllum = false;
    vp.setSamples(100, 4);
    vp.gamma = 1.0;

    /* Materials */
    auto material1P = new Phong(vec3(0.25, 0.72, 0.96), 0.2, 0.4, 0.3);
    auto material2P = new Phong(vec3(0.44, 0.24, 0.61), 0.4, 0.6, 0.1);
    material2P->setSpecularExponent(8.0f);
    auto material3P = new Matte(vec3(1.0), 0.4, 0.6);
    auto material4P = new Emissive(vec3(1.0, 0.82, 0.59), 350000.0);
    auto material5P = new Phong(vec3(0.14, 0.47, 0.8), 0.3, 0.6, 0.1);
    auto material6P = new GlossyReflector(vec3(0.89, 0.36, 0.14), 0.2, 0.2, 0.1, 0.6);
    auto material7P = new Reflective(vec3(1.0), 0.1, 0.2, 0.1, 0.8);
    material7P->setReflectiveSampler(new MultiJittered(100, 2));
    auto material8P = new GlossyReflector(vec3(1.0), 0.2, 0.2, 0.1, 0.7);
    material8P->setGlossyReflectionExponent(100.0f);
    material8P->setGlossyReflectionSampler(new MultiJittered(100, 2));
    auto material9P = new Matte(vec3(0.9, 0.2, 0.13), 0.3, 0.7);
    auto material10P = new Matte(vec3(0.33, 0.78, 0.25), 0.3, 0.7);
    auto material11P = new Reflective(vec3(1.0), 0.2, 0.2, 0.2, 0.4);

    /* Geometry Objects */
    auto sphere1P = new Sphere(material1P);
    sphere1P->setParams(dvec3(-100, 80, 0), 80.0);
    auto sphere2P = new Sphere(material6P);
    sphere2P->setParams(dvec3(0, 60, 0), 60);
    auto plane1P = new Plane(material3P);
    plane1P->setParams(dvec3(0, 1, 0), dvec3(0.0, 0.0, 0.0));
    auto disk1P = new Disk(material4P);
    disk1P->setParams(dvec3(0, 199.9, -100), dvec3(0, -1, 0), 30);
    disk1P->setSampler(new MultiJittered(256, 2));
    disk1P->toggleShadowCast(false);
    auto modelP = new Model("resources/bunny.obj", material3P);
    auto inst1P = new Instance(modelP);
    inst1P->scale(dvec3(50))->translate(dvec3(10, 0, -120));
    auto rect1P = new Rectangle(material8P); // back
    rect1P->setParams(dvec3(-100, 200, -200), dvec3(0, -200, 0), dvec3(200, 0, 0));
    auto rect2P = new Rectangle(material3P); // up
    rect2P->setParams(dvec3(-100, 200, -200), dvec3(200, 0, 0), dvec3(0, 0, 200));
    auto rect3P = new Rectangle(material9P); // left
    rect3P->setParams(dvec3(-100, 200, -200), dvec3(0, 0, 200), dvec3(0, -200, 0));
    auto rect4P = new Rectangle(material11P); // bottom
    rect4P->setParams(dvec3(-100, 0, -200), dvec3(0, 0, 200), dvec3(200, 0, 0));
    auto rect5P = new Rectangle(material10P); // right
    rect5P->setParams(dvec3(100, 0, -200), dvec3(0, 0, 200), dvec3(0, 200, 0));

    // addObject(plane1P);
    addObject(disk1P);
    addObject(inst1P);
    addObject(rect1P);
    addObject(rect2P);
    addObject(rect3P);
    addObject(rect4P);
    addObject(rect5P);

    /* Lights */
    bgColor = vec3();
    AmbientOccluder *occluderP = new AmbientOccluder(vec3(1.0, 0.82, 0.59), 1.0, 0.2);
    occluderP->setSampler(new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS));
    occluderP->setOcclusionSamples(1);
    setAmbient(occluderP);

    auto area1P = new AreaLight(disk1P);
    area1P->toggleShadowCast(true);
    area1P->setAttenuationType(AreaLight::SQUARE);
    addLight(area1P);

    /* Camera & Tracer */
    tracerP = new Whitted(this);
    PinHole *cam = new PinHole(dvec3(0, 100, 210), dvec3(0, 100, 0), 200);
    cameraP = cam;
    _pixels = new unsigned char[vp.horRes * vp.vertRes * vp.numChannels];
    finished = 0;
}