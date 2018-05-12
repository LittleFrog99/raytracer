#include "utilities.h"

bool Math::catmullRomWeights(int size, float *nodes, float x, int *offset, float *weights)
{
    if (x < nodes[0] || x > nodes[size - 1]) return false; // out of bounds
    // Searching for the interval index containing x
    int index = Math::findInterval(size, [&](int i) { return nodes[i] <= x; });
    *offset = index - 1;
    float x0 = nodes[index], x1 = nodes[index + 1];
    // Compute the t paramter and powers
    float t = (x - x0) / (x1 - x0), t2 = t * t, t3 = t2 * t;
    // Compute initial node weights w1 and w2
    weights[1] = 2 * t3 - 3 * t2 + 1;
    weights[2] = -2 * t3 + 3 * t2;
    // Compute first node weight w0
    if (index > 0) {
        float w0 = (t3 - 2 * t2 + t) * (x1 - x0) / (x1 - nodes[index - 1]);
        weights[0] = -w0;
        weights[2] += w0;
    } else {
        float w0 = t3 - 2 * t2 + t;
        weights[0] = 0;
        weights[1] -= w0;
        weights[2] += w0;
    }
    // Compute last node weight w3
    if (index + 2 < size) {
        float w3 = (t3 - t2) * (x1 - x0) / (nodes[index + 2] - x0);
        weights[1] -= w3;
        weights[3] = w3;
    } else {
        float w3 = t3 - t2;
        weights[1] -= w3;
        weights[2] += w3;
        weights[3] = 0;
    }
    return true;
}