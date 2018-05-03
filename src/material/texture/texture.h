#pragma once

#include "core/shade.h"

enum TextureMap {
    RECTANGULAR,
    CYLINDRICAL,
    SPHERICAL,
    LIGHTPROBE,
    MESHUV
};

class Texture {
public:
    enum TextureType {
        DIFFUSE,
        SPECULAR,
        AMBIENT,
        EMISSIVE,
        NORMAL,
        HEIGHT,
        REFLECTION,
        DISPLACEMENT
    };

    Texture() {}
    Texture(TextureMap tex_map) : texMap(tex_map) {}
    virtual vec3 getColor(Shade &shade) = 0;
    inline void setTextureMap(TextureMap tex_map) { texMap = tex_map; }
    virtual ~Texture() {}

protected:
    TextureMap texMap;

    vec2 mapTextureCoord(Shade &shade);
    vec2 rectangularMap(dvec3 local_hit_pt);
    vec2 cylindricalMap(dvec3 local_hit_pt);
    vec2 sphericalMap(dvec3 local_hit_pt);
    vec2 lightProbeMap(dvec3 local_hit_pt);
};

class PureColor : public Texture {
public:
    PureColor(vec3 color) : Texture(RECTANGULAR), color(color) {}
    PureColor(float red, float green, float blue) 
        : Texture(RECTANGULAR), color(red, green, blue) {}
    virtual vec3 getColor(Shade &shade) { return color; }
    operator vec3() { return color; }

private:
    vec3 color;
};
