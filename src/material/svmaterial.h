#pragma once

#include "bsdf/svbsdf.h"
#include "matte.h"
#include "phong.h"

class SVMatte : public Matte {
public:
    SVMatte(Texture *tex, float amb_int, float diff_int);
};

class SVPhong : public Phong {
public:
    SVPhong(Texture *tex, float amb_int, float diff_int, float spec_int);
};