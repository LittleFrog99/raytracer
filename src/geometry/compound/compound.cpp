#include "compound.h"

void Compound::setMaterial(Material *material_ptr) {
    for (int i = 0; i < objects.size(); i++) 
        objects[i]->setMaterial(material_ptr);
}

bool Compound::intersect(Ray &ray, double &tmin, Shade &shade) {
    tmin = numeric_limits<double>::max();
    double t;
    dvec3 normal, hitPt, localHitPt;
    bool hit = false;

    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->intersect(ray, t, shade) && (t < tmin)) {
            hit = true;
            tmin = t;
            materialP = objects[i]->getMaterial();
            normal = shade.normal;
            hitPt = shade.hitPoint;
            localHitPt = shade.localHitPoint;
        }
    }

    if (hit) {
        shade.normal = normal;
        shade.hitPoint = hitPt;
        shade.localHitPoint = localHitPt;
    }
    return hit;
}

bool Compound::shadowIntersect(Ray &ray, double &tmin) {
    tmin = numeric_limits<double>::max();
    double t;
    bool hit = false;

    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->shadowIntersect(ray, t) && (t < tmin)) {
            hit = true;
            tmin = t;
        }
    }
    return hit;
}

Compound::~Compound() {
    for (int i = 0; i < objects.size(); i++)
        delete objects[i];
}