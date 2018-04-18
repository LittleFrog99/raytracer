#pragma once

#include "material/texture/texture.h"
#include "lambertian.h"
#include "specular.h"

class SpatialVarying {
public:
    SpatialVarying(Texture *tex = nullptr) : texture(tex) {}
    vec3 getTexColor(Shade &shade) { return texture->getColor(shade); }
    virtual ~SpatialVarying() { delete texture; }

protected:
    Texture *texture;
};

class SVLambertian : public Lambertian, public SpatialVarying {
public:
    SVLambertian(float intensity, Texture *tex) : Lambertian(intensity), SpatialVarying(tex) {}
    virtual vec3 getColor(Shade &shade) { return SpatialVarying::getTexColor(shade); }
};
