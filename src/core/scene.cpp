#include "world.h"
#include "geometry/primitive/sphere.h"
#include "geometry/primitive/plane.h"
#include "geometry/primitive/box.h"
#include "geometry/primitive/disk.h"
#include "geometry/primitive/rectangle.h"
#include "geometry/instance.h"
#include "geometry/model/svmodel.h"
#include "material/emissive.h"
#include "material/reflective.h"
#include "material/glossyreflector.h"
#include "material/transparent.h"
#include "material/dielectric.h"
#include "material/svmaterial.h"
#include "light/ambientoccluder.h"
#include "light/arealight.h"
#include "camera/pinhole.h"
#include "sampler/multijittered.h"

void World::build() {
    /* Viewplane */
    vp.horRes = 400;
    vp.vertRes = 400;
    vp.pixelSize = 0.005;
    vp.maxDepth = 5;
    vp.illum = PATHTRACING;
    vp.sampleDist = 0.5;
    vp.samplePhotons = 100;
    vp.setSamples(36, DEFAULT_NUM_SETS);
    vp.gamma = 1.0;

    /* Materials */
    auto plastic1P = new Phong(vec3(0.25, 0.72, 0.96), 0.2, 0.4, 0.3);
    auto plastic2P = new Phong(vec3(0.44, 0.24, 0.61), 0.4, 0.6, 0.1);
    plastic2P->setSpecularExponent(8.0f);
    auto plastic3P = new Matte(vec3(1.0), 0.3, 0.7);
    auto emi1P = new Emissive(vec3(1.0), 50.0);
    auto plastic4P = new Phong(vec3(0.14, 0.47, 0.8), 0.3, 0.6, 0.1);
    auto bronzeP = new GlossyReflector(vec3(0.89, 0.36, 0.14), 0.2, 0.2, 0.1, 0.6);
    auto silver1P = new Reflective(vec3(1.0), 0.1, 0.2, 0.1, 0.8);
    auto silver2P = new GlossyReflector(vec3(1.0), 0.2, 0.2, 0.1, 0.7);
    silver2P->setGlossyReflectionExponent(100.0f);
    auto plastic5P = new Matte(vec3(0.9, 0.2, 0.13), 0.3, 0.7);
    auto plastic6P = new Matte(vec3(0.33, 0.78, 0.25), 0.3, 0.7);
    auto plastic7P = new Matte(vec3(0.28, 0.64, 0.93), 0.3, 0.7);
    auto mirror1P = new Reflective(vec3(1.0), 0.2, 0.2, 0.3, 0.6);
    auto glass1P = new Transparent(vec3(1.0), 0.0, 0.0, 0.0, 0.1, 0.9, 1.5);
    auto glass2P = new Dielectric(vec3(0.28, 0.64, 0.93), 0.2, 0.1, 0.1, 1.33, 1.0);
    auto 🐸 = new ImageTexture("resources/frog.jpg", RECTANGULAR);
    auto frogP = new SVPhong(🐸, 0.2, 0.5, 0.3);

    /* Geometry Objects */
    auto sphere1P = new Sphere(plastic1P);
    sphere1P->setParams(dvec3(-1.00, 0.80, 0), .80);
    auto sphere2P = new Sphere(glass1P);
    sphere2P->setParams(dvec3(0.4, 0.30, -.50), .30);
    auto plane1P = new Plane(plastic3P);
    plane1P->setParams(dvec3(0, 1, 0), dvec3(0.0, 0.0, 0.0));
    auto disk1P = new Disk(emi1P);
    disk1P->setParams(dvec3(0, 1.99, -1.00), dvec3(0, -1, 0), 0.30);
    disk1P->setSampler(new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS));
    disk1P->toggleShadowCast(false);
    auto inst2P = new Instance(sphere2P);
    inst2P->scale(dvec3(1.3, 1, 1));
    auto rect1P = new Rectangle(plastic3P); // back
    rect1P->setParams(dvec3(-1.00, 2.00, -2.00), dvec3(0, -2.00, 0), dvec3(2.00, 0, 0));
    auto rect2P = new Rectangle(plastic3P); // up
    rect2P->setParams(dvec3(-1.00, 2.00, -2.00), dvec3(2.00, 0, 0), dvec3(0, 0, 2.00));
    auto rect3P = new Rectangle(plastic7P); // left
    rect3P->setParams(dvec3(-1.00, 2.00, -2.00), dvec3(0, 0, 2.00), dvec3(0, -2.00, 0));
    auto rect4P = new Rectangle(plastic3P); // bottom
    rect4P->setParams(dvec3(-1.00, 0, -2.00), dvec3(0, 0, 2.00), dvec3(2.00, 0, 0));
    auto rect5P = new Rectangle(plastic6P); // right
    rect5P->setParams(dvec3(1.00, 0, -2.00), dvec3(0, 0, 2.00), dvec3(0, 2.00, 0));
    auto rect6P = new Rectangle(frogP);
    auto inst3P = new Instance(rect6P);
    inst3P->scale(dvec3(0.5, 1, 0.5))->rotate(dvec3(1, 0, 0), radians(60.0f))->translate(dvec3(0, 0.6, -0.8));
    SVModel::setMaterialParams(0.4, 0.5, 0.2);
    auto cubeP = new Model("resources/cube/cube.obj", glass1P);
    auto inst4P = new Instance(cubeP);
    inst4P->rotate(dvec3(1, 0, 0), radians(45.0f))->rotate(dvec3(0, 1, 0), radians(30.0f))->translate(dvec3(0, 0.8, -0.8));

    addObject(sphere2P);
    addObject(disk1P);
    addObject(rect1P);
    addObject(rect2P);
    addObject(rect3P);
    addObject(rect4P);
    addObject(rect5P);

    /* Lights */
    setBackgroundColor(vec3(0));
    AmbientOccluder *occluderP = new AmbientOccluder(vec3(1.0), 1.0, 0.2);
    setAmbient(occluderP);

    auto area1P = new AreaLight(disk1P);
    area1P->toggleShadowCast(true);
    addLight(area1P);

    /* Camera */
    auto cam = new PinHole(dvec3(0, 1.00, 2.10), dvec3(0, 1.00, 0), 2.00);
    setCamera(cam);
}
