#include "subsurface.h"

Subsurface::Subsurface(float eta, vec3 albedo, vec3 scatter) {
    SSS = new Tabulated(eta, albedo, scatter, &table);
}
