#pragma once

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
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
static const double EPSILON = 1e-6;
static const unsigned int DEFAULT_NUM_SAMPLES = 100;
static const unsigned int DEFAULT_NUM_SETS = 4;
static const dvec3 UP_VECTOR = dvec3(0.0034, 1, 0.0071);
static const dmat4 IDENTITY_MATRIX_FOUR = dmat4(dvec4(1, 0, 0, 0),
                                           dvec4(0, 1, 0, 0),
                                           dvec4(0, 0, 1, 0),
                                           dvec4(0, 0, 0, 1));
static const dmat3 IDENTITY_MATRIX_THREE = dmat3(dvec3(1, 0, 0), 
                                                dvec3(0, 1, 0),
                                                dvec3(0, 0, 1));

static mutex mut;

namespace Color {
    const vec3 WHITE = vec3(1.0f);
    const vec3 BLACK = vec3(0.0f);
};

class Random {
public:
    Random() {
        srand((unsigned) time(nullptr));
    }

    inline int randomInteger() {
        return rand();
    }

    inline float randomFloat() {
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
static Random rndm;

static float roulette() {
    static unsigned int x = (unsigned) time(nullptr);
    static const unsigned int a = pow(7, 5);
    static const unsigned int c = 1;
    static const unsigned int m = RAND_MAX;
    x = (x * a + c) % m;
    return float(x) / RAND_MAX;
}

namespace Math {
    template <class T>
    inline T distSq(tvec3<T, highp> a, tvec3<T, highp> b) {
        return dot(a - b, a - b);
    }

    template <class T>
    inline T lengSqd(tvec3<T, highp> vec) {
        return dot(vec, vec);
    }

    template <class T>
    inline T maxComp(tvec3<T, highp> vector) {
        return glm::max(glm::max(vector[0], vector[1]), vector[2]);
    }

    template <class T>
    inline T maxComp(tvec3<T, highp> vector, int &index) {
        index = vector[0] > vector[1] ? 0 : 1;
        index = vector[2] > vector[index] ? 2 : index;
        return vector[index];
    }

    template <class T>
    inline T minComp(tvec3<T, highp> vector) {
        return glm::min(glm::min(vector[0], vector[1]), vector[2]);
    }

    template <class T>
    inline T minComp(tvec3<T, highp> vector, int &index) {
        index = vector[0] < vector[1] ? 0 : 1;
        index = vector[2] < vector[index] ? 2 : index;
        return vector[index];
    }

    template<class T>
    inline bool inside(T value, T min, T max) {
        return value > min && value < max;
    }

    inline int findInterval(int size, function<bool(int)> compare) {
        int first = 0, len = size;
        while (len > 0) {
            int half = len >> 1, middle = first + half;
            if (compare(middle)) {
                first = middle + 1;
                len -= half + 1;
            } else 
                len = half;
        }
        return clamp(first - 1, 0, size - 2);
    }

    int solveQuadric(double *coeff, double *solution);
    
    int solveCubic(double *coeff, double *solution);

    int solveQuartic(double *coeff, double *solution);

    bool catmullRomWeights(int size, float *nodes, float x, int *offset, float *weights);

    float sampleCatmullRom2D(int size1, int size2, float *nodes1, float *nodes2, float *values,
        float *cdf, float alpha, float u, float *fval = nullptr, float *pdf = nullptr);

};

namespace Collections {
    template <class T>
    inline void fill(vector<T> &vec, T obj, int num) {
        vec.reserve(num);
        for (int i = 0; i < num; i++) 
            vec[i] = obj;
    }
}
