#include "instance.h"

Instance::Instance(Geometry *obj_ptr) {
    objectP = obj_ptr;
    createBoundingBox(obj_ptr->getBoundingBox());
}

Instance * Instance::transform(dmat4 trans) {
    matrix = trans * matrix;
    invMat = inverse(matrix);
    normalMat = transpose(dmat3(invMat));
    createBoundingBox(calcBoundingBox());
    return this;
}

Instance * Instance::translate(dvec3 dir) {
    transform(glm::translate(IDENTITY_MATRIX_FOUR, dir));
    return this;
}

Instance * Instance::rotate(dvec3 axis, double angle) {
    transform(glm::rotate(IDENTITY_MATRIX_FOUR, angle, axis));
    return this;
}

Instance * Instance::scale(dvec3 scale) {
    transform(glm::scale(IDENTITY_MATRIX_FOUR, scale));
    return this;
}

BoundingBox Instance::calcBoundingBox() {
    dvec3 vertices[8];
    dvec3 min = bndBox.vertMin, max = bndBox.vertMax;
    vertices[0] = min;
    vertices[1] = dvec3(min.x, min.y, max.z);
    vertices[2] = dvec3(min.x, max.y, min.z);
    vertices[3] = dvec3(min.x, max.y, max.z);
    vertices[4] = dvec3(max.x, min.y, min.z);
    vertices[5] = dvec3(max.x, min.y, max.z);
    vertices[6] = dvec3(max.x, max.y, min.z);
    vertices[7] = max;

    min = dvec3(numeric_limits<double>::max()), max = dvec3(numeric_limits<double>::min());
    for (dvec3 vert : vertices) {
        dvec3 newVert = dvec3(matrix * dvec4(vert, 1.0));
        for (int t = 0; t < 3; t++) {
            min[t] = glm::min(newVert[t], min[t]);
            max[t] = glm::max(newVert[t], max[t]);
        }
    }
    return BoundingBox(min, max);
}

bool Instance::intersect(Ray &ray, double &tmin, Shade &shade) {
    dvec4 invOrigin = invMat * dvec4(ray.origin, 1.0);
    dvec4 invDir = invMat * dvec4(ray.direction, 0.0);
    Ray invRay = Ray(dvec3(invOrigin), dvec3(invDir));

    if (objectP->intersect(invRay, tmin, shade)) {
        shade.normal = normalize(normalMat * shade.normal);
        return true;
    }
    else return false;
}

bool Instance::shadowIntersect(Ray &ray, double &tmin) {
    dvec4 invOrigin = invMat * dvec4(ray.origin, 1.0);
    dvec4 invDir = invMat * dvec4(ray.direction, 0.0);
    Ray invRay = Ray(dvec3(invOrigin), dvec3(invDir));

    if (objectP->shadowIntersect(invRay, tmin)) 
        return true;
    else return false;
}
