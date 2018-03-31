#include "solidcylinder.h"
#include "geometry/primitive/opencylinder.h"
#include "geometry/primitive/disk.h"

SolidCylinder::SolidCylinder(Material *mat_ptr) {
    addObject(new Disk()); // objects[0]
    addObject(new Disk()); // objects[1]
    addObject(new OpenCylinder()); // objects[2]
    setMaterial(mat_ptr);
}

void SolidCylinder::setParams(double bottom_pos, double top_pos, double radius) {
    auto bottomDisk = (Disk *) objects[0];
    bottomDisk->setParams(dvec3(0, bottom_pos, 0), dvec3(0, -1, 0), radius);
    auto topDisk = (Disk *) objects[1];
    topDisk->setParams(dvec3(0, top_pos, 0), dvec3(0, 1, 0), radius);
    auto cylinder = (OpenCylinder *) objects[2];
    cylinder->setParams(bottom_pos, top_pos, radius);

    bndBox = BoundingBox(dvec3(-radius, bottom_pos, -radius), 
                         dvec3(radius, top_pos, radius));
}

inline bool SolidCylinder::intersect(Ray &ray, double &tmin, Shade &shade) {
    return bndBox.intersect(ray) && Compound::intersect(ray, tmin, shade);
}

inline bool SolidCylinder::shadowIntersect(Ray &ray, double &tmin) {
    return bndBox.intersect(ray) && Compound::shadowIntersect(ray, tmin);
}