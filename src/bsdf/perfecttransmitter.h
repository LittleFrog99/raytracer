#pragma once

#include "core/btdf.h"

class PerfectTransmitter : public BTDF {
public:
    PerfectTransmitter() {}
    PerfectTransmitter(float intensity, float ior);
    virtual vec3 sampleBTDF(Shade &shade, dvec3 &trans, dvec3 &out);
    virtual bool isTIR(Shade &shade);

    inline void setColor(vec3 color) {
        this->color = color;
    }

private:
    float intensity, ior;
    vec3 color = vec3(1.0);
};