#pragma once

#include "material/texture/texture.h"
#include "lambertian.h"
#include "specular.h"

class SVBSDF {
public:
    SVBSDF(Texture *tex = nullptr) : textureP(tex) {}
    void setTexture(Texture *tex_ptr) { textureP = tex_ptr; }
    virtual ~SVBSDF() { if (textureP) delete textureP; }

protected:
    Texture *textureP;
    vec3 getTextureColor(Shade &shade) { return textureP->getColor(shade); }
};

class SVLambertian : public Lambertian, public SVBSDF {
public:
    SVLambertian(float intensity, Texture *tex) : Lambertian(intensity), SVBSDF(tex) {}
    virtual vec3 getColor(Shade &shade) { return getTextureColor(shade); }
};

class SVSpecular : public Specular, public SVBSDF {
public:
    SVSpecular(float intensity, Texture *tex, float exponent = DEFAULT_SPECULAR_EXPONENT) 
        : Specular(intensity, Color::WHITE, exponent), SVBSDF(tex) {}
    virtual vec3 getColor(Shade &shade) { return getTextureColor(shade); }
};
