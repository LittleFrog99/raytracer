#pragma once

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

static const double PI = 3.14159265358979323846;
static const double INV_PI = 1.0 / PI;
static const dvec3 UP_VECTOR = dvec3(0.0034, 1, 0.0071);

static mutex mut;

class Random {
public:
    Random(unsigned int seed = time(nullptr)) {
        srand(seed);
    }

    inline int randomInteger() {
        return rand();
    }

    inline double randomFloat() {
        return float(rand()) / RAND_MAX;
    }

    inline double randomDouble() {
        return double(rand()) / RAND_MAX;
    }

    inline double randomDouble(double low, double high) {
        return randomDouble() * (high - low) + low;
    }

    inline int randomInteger(int low, int high) {
        return int(randomDouble(0, high - low + 1) + low);
    }

};

namespace Debug {
    template <class T>
    inline void log(tvec3<T, highp> vec3) {
        cout << "X: " << vec3.x << " Y: " << vec3.y << " Z: " << vec3.z << endl;
    }
};

namespace Math {
    template <class T>
    inline T distanceSquared(tvec3<T, highp> a, tvec3<T, highp> b) {
        return dot(a - b, a - b);
    }

    template <class T>
    inline T lengthSquared(tvec3<T, highp> vec) {
        return dot(vec, vec);
    }

    template <class T>
    inline T maxComponent(tvec3<T, highp> vector) {
        return glm::max(glm::max(vector[0], vector[1]), vector[2]);
    }

    template <class T>
    inline T maxComponent(tvec3<T, highp> vector, int &index) {
        index = vector[0] > vector[1] ? 0 : 1;
        index = vector[2] > vector[index] ? 2 : index;
        return vector[index];
    }

    template <class T>
    inline T minComponent(tvec3<T, highp> vector) {
        return glm::min(glm::min(vector[0], vector[1]), vector[2]);
    }

    template <class T>
    inline T minComponent(tvec3<T, highp> vector, int &index) {
        index = vector[0] < vector[1] ? 0 : 1;
        index = vector[2] < vector[index] ? 2 : index;
        return vector[index];
    }

    int solveQuadric(double *coeff, double *solution);
    
    int solveCubic(double *coeff, double *solution);

    int solveQuartic(double *coeff, double *solution);
};

