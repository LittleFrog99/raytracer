#include "compound.h"

void Compound::setMaterial(Material *material_ptr) {
    for (Geometry *objP : objects) 
        objP->setMaterial(material_ptr);
}

bool Compound::intersect(Ray &ray, double &tmin, Shade &shade) {
    tmin = numeric_limits<double>::max();
    double t;
    dvec3 normal, hitPt, localHitPt;
    vec2 texCoord;
    Material *matP;
    bool hit = false;

    for (Geometry *objP : objects) {
        if (objP->intersect(ray, t, shade) && (t < tmin)) {
            hit = true;
            tmin = t;
            matP = shade.materialP;
            normal = shade.normal;
            localHitPt = shade.localHitPoint;
            texCoord = shade.texCoord;
        }
    }

    if (hit) {
        shade.normal = normal;
        shade.localHitPoint = localHitPt;
        shade.materialP = matP;
        shade.texCoord = texCoord;
    }
    return hit;
}

bool Compound::shadowIntersect(Ray &ray, double &tmin) {
    tmin = numeric_limits<double>::max();
    double t;
    bool hit = false;

    for (Geometry *objP : objects) {
        if (objP->shadowIntersect(ray, t) && (t < tmin)) {
            hit = true;
            tmin = t;
        }
    }
    return hit;
}

BoundingBox Compound::calcBoundingBox() {
    dvec3 min = dvec3(numeric_limits<double>::max());
    dvec3 max = dvec3(numeric_limits<double>::min());
    for (Geometry *objP : objects) {
        BoundingBox bnd = objP->getBoundingBox();
        for (int t = 0; t < 3; t++) {
            min[t] = glm::min(min[t], bnd.vertMin[t]);
            max[t] = glm::max(max[t], bnd.vertMax[t]);
        }
    }
    return BoundingBox(min, max);
}

Compound::~Compound() {
    for (Geometry *objP : objects)
        delete objP;
}