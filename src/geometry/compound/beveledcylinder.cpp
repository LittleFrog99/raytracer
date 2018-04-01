#include "beveledcylinder.h"
#include "geometry/primitive/opencylinder.h"
#include "geometry/primitive/disk.h"
#include "geometry/primitive/torus.h"
#include "geometry/instance.h"

BeveledCylinder::BeveledCylinder(Material *mat_ptr) {
    addObject(new Disk()); // objects[0]
    addObject(new Disk()); // objects[1]
    addObject(new OpenCylinder()); // objects[2]
    addObject(new Instance(new Torus())); // objects[3]
    addObject(new Instance(new Torus())); // objects[4]
    setMaterial(mat_ptr);
}

void BeveledCylinder::setParams(double bottom_pos, double top_pos, double radius, double bevel_radius) {
    auto bottomDisk = (Disk *) objects[0];
    bottomDisk->setParams(dvec3(0, bottom_pos, 0), dvec3(0, -1, 0), radius - bevel_radius);

    auto topDisk = (Disk *) objects[1];
    topDisk->setParams(dvec3(0, top_pos, 0), dvec3(0, 1, 0), radius - bevel_radius);

    auto cylinder = (OpenCylinder *) objects[2];
    cylinder->setParams(bottom_pos + bevel_radius, top_pos - bevel_radius, radius);

    auto bottomTorusInst = (Instance *) objects[3];
    bottomTorusInst->translate(dvec3(0, bottom_pos + bevel_radius, 0));
    ((Torus *) bottomTorusInst->getObject())->setParams(radius - bevel_radius, bevel_radius);

    auto topTorusInst = (Instance *) objects[4];
    topTorusInst->translate(dvec3(0, top_pos - bevel_radius, 0));
    ((Torus *) topTorusInst->getObject())->setParams(radius - bevel_radius, bevel_radius);

    bndBox = BoundingBox(dvec3(-radius, bottom_pos, -radius), 
                         dvec3(radius, top_pos, radius));
}

inline bool BeveledCylinder::intersect(Ray &ray, double &tmin, Shade &shade) {
    return bndBox.intersect(ray) && Compound::intersect(ray, tmin, shade);
}

inline bool BeveledCylinder::shadowIntersect(Ray &ray, double &tmin) {
    return bndBox.intersect(ray) && Compound::shadowIntersect(ray, tmin);
}