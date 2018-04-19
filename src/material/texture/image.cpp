#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(string path) {
    data = stbi_load(path.c_str(), &resolution.x, &resolution.y, &numChannels, 0);
}

vec3 Image::getColor(vec2 texCoord) {
    ivec2 pixelCoord = vec2(resolution - ivec2(1)) * texCoord;
    int index = pixelCoord.y * resolution.x + pixelCoord.x;

    vec3 color;
    float CHANNEL_MAX = numeric_limits<unsigned char>::max();
    color.r = float(data[index * numChannels]) / CHANNEL_MAX;
    color.g = float(data[index * numChannels + 1]) / CHANNEL_MAX;
    color.b = float(data[index * numChannels + 2]) / CHANNEL_MAX;

    return color;
}