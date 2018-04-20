#include "world.h"
#include "core/shade.h"
#include "core/material.h"
#include "tracer/globaltracer.h"
#include "utilities.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Shade World::intersectObjects(Ray &ray) {
    Shade shade(*this);
    double t;
    double tmin = numeric_limits<double>::max();
    Material *matP;
    dvec3 normal, hitPoint, localHitPoint;
    vec2 texCoord;

    for (Geometry *objP : objects) {
        if (objP->intersect(ray, t, shade) && (t < tmin)) {
            shade.hasHit = true;
            tmin = t;
            matP = shade.materialP;
            hitPoint = ray.origin + t * ray.direction;
            normal = shade.normal;
            localHitPoint = shade.localHitPoint;
            texCoord = shade.texCoord;
        }
    }

    if (shade.hasHit) { // only the nearest object
        shade.t = tmin;
        shade.normal = normal;
        shade.hitPoint = hitPoint;
        shade.localHitPoint = localHitPoint;
        shade.materialP = matP;
        shade.texCoord = texCoord;
    }

    return shade;
}

void World::setup() {
    build();
    
    tracerP = new GlobalTracer(this);
    _pixels = new unsigned char[vp.horRes * vp.vertRes * vp.numChannels];
    finished = 0;
}

void World::displayStatus() {
    unsigned long total = vp.horRes * vp.vertRes;
    while (finished <= total) {
        cout << '\r';
        cout << setw(8) << setprecision(4) << double(finished) * 100 / total << '%';
        cout.flush();
        if (finished == total)
            break;
        usleep(100000);
    }
}

void World::render() {
    thread status(&World::displayStatus, this);
    status.detach();
    cameraP->render(*this);
    usleep(100000);
}

void World::plotPoint(int row, int col, vec4 color) {
    color = vec4(vec3(color), color.a);

    static float MAX = numeric_limits<unsigned char>::max();
    unsigned int offset = vp.numChannels * vp.horRes * row + vp.numChannels * col;
    _pixels[offset] = static_cast<unsigned char>(MAX * color.r);
    _pixels[offset + 1] = static_cast<unsigned char>(MAX * color.g);
    _pixels[offset + 2] = static_cast<unsigned char>(MAX * color.b);
    _pixels[offset + 3] = static_cast<unsigned char>(MAX * color.a);

    finished++;
}

void World::output(string path) {
    stbi__flip_vertically_on_write = 1;
    stbi_write_png(path.c_str(), vp.horRes, vp.vertRes, vp.numChannels, _pixels, 0);
}

World::~World() {
    delete tracerP;
    delete cameraP;
    delete ambientP;
    for (auto objP : objects)
        delete objP;
    for (auto lightP : lights)
        delete lightP;
    delete _pixels;
}
