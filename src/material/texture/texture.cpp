#include "texture.h"

vec2 Texture::mapTextureCoord(Shade &shade) {
    switch (texMap) {
        case RECTANGULAR: return rectangularMap(shade.localHitPoint);
        case MESHUV: return shade.texCoord;
        case CYLINDRICAL : return cylindricalMap(shade.localHitPoint);
        case SPHERICAL: return sphericalMap(shade.localHitPoint);
        case LIGHTPROBE: return lightProbeMap(shade.localHitPoint);
    }
}

vec2 Texture::rectangularMap(glm::dvec3 local_hit_pt) {
    return vec2(local_hit_pt.x + 1, local_hit_pt.z + 1) / 2.0f;
}

vec2 Texture::cylindricalMap(dvec3 local_hit_pt) {
    float azim = atan2(local_hit_pt.x, local_hit_pt.z);
    if (azim < 0.0) azim += 2.0 * PI;

    float u = azim / (2.0 * PI);
    float v = (local_hit_pt.y + 1.0) / 2.0;
    return vec2(u, v);
}

vec2 Texture::sphericalMap(dvec3 local_hit_pt) {
    float polar = acos(local_hit_pt.y);
    float azim = atan2(local_hit_pt.x, local_hit_pt.z);
    if (azim < 0.0) azim += 2.0 * PI;

    float u = azim / (2.0 * PI);
    float v = 1.0 - polar / PI;
    return vec2(u, v);
}

vec2 Texture::lightProbeMap(dvec3 local_hit_pt) {
    float alpha = acos(local_hit_pt.z);
    float sinBeta = local_hit_pt.y / length(vec2(local_hit_pt.x, local_hit_pt.y));
    float cosBeta = local_hit_pt.x / length(vec2(local_hit_pt.x, local_hit_pt.y));

    float u = (1.0 + cosBeta * alpha / PI) / 2.0;
    float v = (1.0 + sinBeta * alpha / PI) / 2.0;
    return vec2(u, v);
}
