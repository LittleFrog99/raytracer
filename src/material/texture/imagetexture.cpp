#include "imagetexture.h"

ImageTexture::ImageTexture(Image *image_ptr, TextureMap tex_map)
    : Texture(tex_map), imageP(image_ptr) {}

ImageTexture::ImageTexture(string path, TextureMap tex_map) 
    : Texture(tex_map) 
{
    imageP = new Image(path);
}

vec3 ImageTexture::getColor(Shade &shade) {
    return imageP->getColor(mapTextureCoord(shade));
}
