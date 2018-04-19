#pragma once

#include "texture.h"
#include "image.h"

class ImageTexture : public Texture {
public:
    ImageTexture(string path, TextureMap tex_map); // load from file
    ImageTexture(Image *image_ptr, TextureMap tex_map);
    virtual vec3 getColor(Shade &shade);
    virtual ~ImageTexture() { if (imageP) delete imageP; }

private:
    Image *imageP;
};