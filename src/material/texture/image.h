#pragma once

#include "utilities.h"

class Image {
public:
    Image(string path);
    virtual vec3 getColor(int row, int column);

private:
    int vertRes, horRes;
    unsigned char *data;

    void processImage(string path);
};