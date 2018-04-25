cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

project(raytracer)

include_directories(/usr/local/include)
include_directories(src/utilities)
include_directories(src)

find_library(ASSIMP assimp)

set(SOURCES_CORE src/main.cpp 
    src/core/world.cpp src/core/viewplane.cpp
    src/core/sampler.cpp src/core/camera.cpp 
    src/core/scene.cpp src/core/geometry.cpp)
set(SOURCES_GEOMETRY src/geometry/primitive/plane.cpp 
    src/geometry/primitive/sphere.cpp src/geometry/primitive/rectangle.cpp
    src/geometry/primitive/box.cpp src/geometry/primitive/triangle.cpp 
    src/geometry/primitive/disk.cpp src/geometry/primitive/opencylinder.cpp 
    src/geometry/primitive/torus.cpp src/geometry/bound/boundingbox.cpp
    src/geometry/part/partsphere.cpp src/geometry/part/parttorus.cpp
    src/geometry/compound/compound.cpp src/geometry/compound/solidcylinder.cpp
    src/geometry/instance.cpp src/geometry/compound/beveledcylinder.cpp
    src/geometry/compound/grid.cpp src/geometry/model/meshtriangle.cpp
    src/geometry/model/smoothmeshtriangle.cpp src/geometry/model/model.cpp
    src/geometry/model/svmodel.cpp)
set(SOURCES_TRACER src/tracer/globaltracer.cpp)
set(SOURCES_SAMPLER src/sampler/regular.cpp src/sampler/multijittered.cpp)
set(SOURCES_CAMERA src/camera/thinlens.cpp src/camera/pinhole.cpp)
set(SOURCES_BSDF src/bsdf/lambertian.cpp src/bsdf/specular.cpp
    src/bsdf/perfectspecular.cpp src/bsdf/perfecttransmitter.cpp
    src/bsdf/fresnelreflector.cpp src/bsdf/fresneltransmitter.cpp)
set(SOURCES_MATERIAL src/material/matte.cpp src/material/phong.cpp 
    src/material/emissive.cpp src/material/reflective.cpp
    src/material/glossyreflector.cpp src/material/transparent.cpp
    src/material/dielectric.cpp src/material/texture/image.cpp
    src/material/texture/texture.cpp src/material/svmaterial.cpp
    src/material/texture/imagetexture.cpp)
set(SOURCES_LIGHT src/light/ambientoccluder.cpp src/light/arealight.cpp
    src/light/pointlight.h)
set(SOURCES_UTILITIES src/utilities/equations.cpp)
set(SOURCES_PHOTONMAP src/photon/photonmap.cpp)

set(SOURCES ${SOURCES_CORE} ${SOURCES_GEOMETRY} ${SOURCES_TRACER} ${SOURCES_SAMPLER} 
    ${SOURCES_CAMERA} ${SOURCES_LIGHT} ${SOURCES_BSDF} ${SOURCES_MATERIAL} ${SOURCES_LIGHT}
    ${SOURCES_UTILITIES} ${SOURCES_PHOTONMAP})
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${ASSIMP})
