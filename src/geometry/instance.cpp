#include "instance.h"

Instance * Instance::transform(dmat4 trans) {
    matrix = trans * matrix;
    invMat = inverse(matrix);
    normalMat = transpose(dmat3(invMat));
    return this;
}

Instance * Instance::translate(dvec3 dir) {
    transform(glm::translate(IDENTITY_MATRIX, dir));
    return this;
}

Instance * Instance::rotate(dvec3 axis, double angle) {
    transform(glm::rotate(IDENTITY_MATRIX, angle, axis));
    return this;
}

Instance * Instance::scale(dvec3 scale) {
    transform(glm::scale(IDENTITY_MATRIX, scale));
    return this;
}

bool Instance::intersect(Ray &ray, double &tmin, Shade &shade) {
    dvec4 invOrigin = invMat * dvec4(ray.origin, 1.0);
    dvec4 invDir = invMat * dvec4(ray.direction, 0.0);
    Ray invRay = { dvec3(invOrigin), normalize(dvec3(invDir)) };

    if (objectP->intersect(invRay, tmin, shade)) {
        shade.normal = normalize(normalMat * shade.normal);
        shade.hitPoint = dvec3(matrix * dvec4(shade.hitPoint, 1.0));
        if (objectP->getMaterial())
            materialP = objectP->getMaterial();
        return true;
    }
    else return false;
}

bool Instance::shadowIntersect(Ray &ray, double &tmin) {
    dvec4 invOrigin = invMat * dvec4(ray.origin, 1.0);
    dvec4 invDir = invMat * dvec4(ray.direction, 0.0);
    Ray invRay = { dvec3(invOrigin), normalize(dvec3(invDir)) };

    if (objectP->shadowIntersect(invRay, tmin)) 
        return true;
    else return false;
}
