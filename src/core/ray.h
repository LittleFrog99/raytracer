#pragma once

#include <glm/glm.hpp>
using namespace glm;

struct Ray {
public:
    dvec4 origin;
    dvec4 direction;
    Ray() {};
    Ray(const dvec4 &ori, const dvec4 &dir) : origin(ori), direction(dir) {}
    Ray & operator = (const Ray &ray);
};
