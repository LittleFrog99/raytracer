#pragma once

#include "core/btdf.h"

class PerfectTransmitter : public BTDF {
public:
    PerfectTransmitter() {}
    PerfectTransmitter(float intensity, float ior);
    virtual vec3 sampleBTDF(Shade &shade, dvec3 &trans, dvec3 &out);
    virtual bool isTIR(Shade &shade);
    virtual vec3 getColor(Shade &shade) { return color; }

    inline void setColor(vec3 color) {
        this->color = color;
    }

protected:
    float intensity;
    vec3 color = vec3(1.0);

private:
    float ior;
};