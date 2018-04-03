#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace Debug {
    template <class T>
    inline void log(tvec3<T, highp> &vec3) {
        cout << "X: " << vec3.x << " Y: " << vec3.y << " Z: " << vec3.z << endl;
    }
};
