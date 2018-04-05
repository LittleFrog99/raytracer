#include "grid.h"

dvec3 Grid::minBounds() {
    dvec3 minCoord(numeric_limits<double>::max());
    for (Geometry *objP : objects) {
        dvec3 minVert = objP->getBoundingBox().vertMin;
        minCoord.x = glm::min(minCoord.x, minVert.x);
        minCoord.y = glm::min(minCoord.y, minVert.y);
        minCoord.z = glm::min(minCoord.z, minVert.z);
    }
    return minCoord - dvec3(EPSILON);
}

dvec3 Grid::maxBounds() {
    dvec3 maxCoord(numeric_limits<double>::min());
    for (Geometry *objP : objects) {
        dvec3 maxVert = objP->getBoundingBox().vertMax;
        maxCoord.x = glm::max(maxCoord.x, maxVert.x);
        maxCoord.y = glm::max(maxCoord.y, maxVert.y);
        maxCoord.z = glm::max(maxCoord.z, maxVert.z);
    }
    return maxCoord + dvec3(EPSILON);
}

void Grid::setupCells() {
    dvec3 min = minBounds(), max = maxBounds();
    createBoundingBox(min, max);

    int numObjs = objects.size();
    dvec3 ext = max - min;
    double step = pow(ext.x * ext.y * ext.z / numObjs, 1.0 / 3.0);
    numCells = ivec3(GRID_MULTIPLIER * ext.x / step + 1,
                     GRID_MULTIPLIER * ext.y / step + 1, 
                     GRID_MULTIPLIER * ext.z / step + 1);
    int totalCells = numCells.x * numCells.y * numCells.z;
    Collections::fill(cells, static_cast<Geometry *>(nullptr), totalCells);

    vector<int> counts;
    Collections::fill(counts, 0, totalCells);

    for (Geometry *objP : objects) {
        BoundingBox objBnd = objP->getBoundingBox();

        ivec3 indexMin, indexMax;
        for (int t = 0; t < 3; t++) {
            indexMin[t] = glm::clamp(
                (objBnd.vertMin[t] - min[t]) * numCells[t] / (max[t] - min[t]), 
                0.0, double(numCells[t] - 1));
            indexMax[t] = glm::clamp(
                (objBnd.vertMax[t] - min[t]) * numCells[t] / (max[t] - min[t]),
                0.0, double(numCells[t] - 1));
        }

        for (int iz = indexMin.z; iz <= indexMax.z; iz++)
            for (int iy = indexMin.y; iy <= indexMax.y; iy++) 
                for (int ix = indexMin.x; ix <= indexMax.x; ix++) {
                    int index = gridCoordsToIndex(ix, iy, iz);

                    if (counts[index] == 0) // no object before, add current object pointer
                        cells[index] = objP;
                    else if (counts[index] == 1) { // one object exists, create a compound, add the 
                    // previously existing object and current object to the compound
                        auto compound_ptr = new Compound();
                        compound_ptr->addObject(cells[index]);
                        compound_ptr->addObject(objP);
                        cells[index] = compound_ptr;
                    } 
                    else { // counts[index] > 1, must be a compound object
                        auto compound_ptr = dynamic_cast<Compound *>(cells[index]);
                        compound_ptr->addObject(objP);
                    }
                    counts[index]++;
                }
    }

    objects.erase(objects.begin(), objects.end());
    counts.erase(counts.begin(), counts.end());
}

bool Grid::intersect(Ray &ray, double &tmin, Shade &shade) {
    dvec3 tMin, tMax;
    for (int i = 0; i < 3; i++) {
        double invComp = 1.0 / ray.direction[i];
        tMin[i] = (invComp >= 0 ? (bndBox.vertMin[i] - ray.origin[i])
                                : (bndBox.vertMax[i] - ray.origin[i])) * invComp;
        tMax[i] = (invComp >= 0 ? (bndBox.vertMax[i] - ray.origin[i])
                                : (bndBox.vertMin[i] - ray.origin[i])) * invComp;
    }

    double tIn = Math::maxComponent(tMin);
    double tOut = Math::minComponent(tMax);
    if (tIn > tOut) return false;

    ivec3 index;
    if (bndBox.inside(ray.origin)) 
        index = calcGridCoords(ray.origin);
    else {
        dvec3 bndHitPt = ray.origin + tIn * ray.direction;
        index = calcGridCoords(bndHitPt);
    }

    dvec3 deltaT = (tMax - tMin) / dvec3(numCells);
    dvec3 tNext;
    ivec3 indexStop, indexStep;

    for (int i = 0; i < 3; i++) {
        double component = ray.direction[i];
        if (component > 0.0) {
            tNext[i] = tMin[i] + (index[i] + 1) * deltaT[i];
            indexStep[i] = +1;
            indexStop[i] = numCells[i];
        }
        else if (component < 0.0) {
            tNext[i] = tMin[i] + (numCells[i] - index[i]) * deltaT[i];
            indexStep[i] = -1;
            indexStop[i] = -1;
        }
        else {
            tNext[i] = numeric_limits<double>::max();
            indexStep[i] = -1;
            indexStop[i] = -1;
        }
    }

    while (true) {
        auto objP = cells[gridCoordsToIndex(index)];

        for (int i = 0; i < 3; i++) 
            if (Math::minComponent(tNext) == tNext[i]) {
                if (objP && objP->intersect(ray, tmin, shade) && tmin < tNext[i]) {
                    materialP = objP->getMaterial();
                    return true;
                }
                tNext[i] += deltaT[i];
                index[i] += indexStep[i];
                if (index[i] == indexStop[i]) return false;
            }
           
    }
}

bool Grid::shadowIntersect(Ray &ray, double &tmin) {
    dvec3 tMin, tMax;
    for (int i = 0; i < 3; i++) {
        double invComp = 1.0 / ray.direction[i];
        tMin[i] = (invComp >= 0 ? (bndBox.vertMin[i] - ray.origin[i])
                                : (bndBox.vertMax[i] - ray.origin[i])) * invComp;
        tMax[i] = (invComp >= 0 ? (bndBox.vertMax[i] - ray.origin[i])
                                : (bndBox.vertMin[i] - ray.origin[i])) * invComp;
    }

    double tIn = Math::maxComponent(tMin);
    double tOut = Math::minComponent(tMax);
    if (tIn > tOut) return false;

    ivec3 index;
    if (bndBox.inside(ray.origin)) 
        index = calcGridCoords(ray.origin);
    else {
        dvec3 bndHitPt = ray.origin + tIn * ray.direction;
        index = calcGridCoords(bndHitPt);
    }

    dvec3 deltaT = (tMax - tMin) / dvec3(numCells);
    dvec3 tNext;
    ivec3 indexStop, indexStep;

    for (int i = 0; i < 3; i++) {
        double component = ray.direction[i];
        if (component > 0) {
            tNext[i] = tMin[i] + (index[i] + 1) * deltaT[i];
            indexStep[i] = 1;
            indexStop[i] = numCells[i];
        }
        else if (component < 0) {
            tNext[i] = tMin[i] + (numCells[i] - index[i]) * deltaT[i];
            indexStep[i] = -1;
            indexStop[i] = -1;
        }
        else {
            tNext[i] = numeric_limits<double>::max();
            indexStep[i] = -1;
            indexStop[i] = -1;
        }
    }

    while (true) {
        auto objP = cells[gridCoordsToIndex(index)];

        for (int i = 0; i < 3; i++) 
            if (Math::minComponent(tNext) == tNext[i]) {
                if (objP && objP->shadowIntersect(ray, tmin) && tmin < tNext[i]) {
                    materialP = objP->getMaterial();
                    return true;
                }
                tNext[i] += deltaT[i];
                index[i] += indexStep[i];
                if (index[i] == indexStop[i]) return false;
            }
           
    }
}

ivec3 Grid::calcGridCoords(dvec3 &point) {
    ivec3 index;
    for (int i = 0; i < 3; i++)
        index[i] = glm::clamp((point[i] - bndBox.vertMin[i]) * numCells[i] /
                                  (bndBox.vertMax[i] - bndBox.vertMin[i]),
                              0.0, double(numCells[i] - 1));
    return index;
}

ivec3 Grid::indexToGridCoords(int index) {
    ivec3 coords;
    coords.z = index / (numCells.x * numCells.y);
    coords.y = index % (numCells.x * numCells.y) / numCells.x;
    coords.x = index % (numCells.x * numCells.y) % (numCells.x);
    return coords;
}

int Grid::gridCoordsToIndex(ivec3 coord) {
    return coord.x + numCells.x * coord.y + numCells.x * numCells.y * coord.z;
}
