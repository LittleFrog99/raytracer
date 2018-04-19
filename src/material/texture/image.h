#pragma once

#include "utilities.h"

class Image {
public:
    Image(string path);
    vec3 getColor(vec2 texCoord);
    ivec2 getResolution() { return resolution; }

private:
    ivec2 resolution;
    int numChannels;
    unsigned char *data;
};