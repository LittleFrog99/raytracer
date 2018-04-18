#pragma once

#include "core/shade.h"
#include "material/texture/image.h"

class Texture {
public:
    virtual vec3 getColor(Shade &shade) = 0;
};

class PureColor : public Texture {
public:
    PureColor(vec3 color) : color(color) {}
    PureColor(float red, float green, float blue) : color(red, green, blue) {}
    virtual vec3 getColor(Shade &shade) { return color; }
    operator vec3() { return color; }

private:
    vec3 color;
};
