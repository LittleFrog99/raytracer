#pragma once

#include "geometry/compound/compound.h"
#include "geometry/bound/boundingbox.h"

class Grid : public Compound, public BoxBounded {
public:
    static constexpr int GRID_MULTIPLIER = 2;

    Grid() {}
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    void setupCells();

private:
    vector<Geometry *> cells;
    ivec3 numCells;

    dvec3 minBounds();
    dvec3 maxBounds();
    ivec3 calcGridCoords(dvec3 &point);
    ivec3 indexToGridCoords(int index);
    int gridCoordsToIndex(ivec3 coord);

    inline int gridCoordsToIndex(int x, int y, int z) { 
        return gridCoordsToIndex(ivec3(x, y, z)); 
    }
};
