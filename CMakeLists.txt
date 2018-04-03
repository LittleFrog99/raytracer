cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

project(raytracer)

include_directories(/usr/local/include)
include_directories(src/utilities)
include_directories(src)

set(SOURCES_CORE src/main.cpp 
    src/core/world.cpp src/core/viewplane.cpp
    src/core/sampler.cpp src/core/camera.cpp 
    src/core/scene.cpp)
set(SOURCES_GEOMETRY src/geometry/primitive/plane.cpp 
    src/geometry/primitive/sphere.cpp src/geometry/primitive/rectangle.cpp
    src/geometry/primitive/box.cpp src/geometry/primitive/triangle.cpp 
    src/geometry/primitive/disk.cpp src/geometry/primitive/opencylinder.cpp 
    src/geometry/primitive/torus.cpp src/geometry/bound/boundingbox.cpp
    src/geometry/part/partsphere.cpp src/geometry/part/parttorus.cpp
    src/geometry/compound/compound.cpp src/geometry/compound/solidcylinder.cpp
    src/geometry/instance.cpp src/geometry/compound/beveledcylinder.cpp
    src/geometry/compound/grid.cpp)
set(SOURCES_TRACER src/tracer/raycast.cpp src/tracer/arealighttracer.cpp)
set(SOURCES_SAMPLER src/sampler/regular.cpp src/sampler/multijittered.cpp)
set(SOURCES_CAMERA src/camera/thinlens.cpp src/camera/pinhole.cpp)
set(SOURCES_BRDF src/brdf/lambertian.cpp src/brdf/specular.cpp)
set(SOURCES_MATERIAL src/material/matte.cpp src/material/phong.cpp 
    src/material/emissive.cpp)
set(SOURCES_LIGHT src/light/ambientoccluder.cpp src/light/arealight.cpp)
set(SOURCES_UTILITIES src/utilities/equations.cpp)

set(SOURCES ${SOURCES_CORE} ${SOURCES_GEOMETRY} ${SOURCES_TRACER} ${SOURCES_SAMPLER} 
    ${SOURCES_CAMERA} ${SOURCES_LIGHT} ${SOURCES_BRDF} ${SOURCES_MATERIAL} ${SOURCES_LIGHT}
    ${SOURCES_UTILITIES})
add_executable(${PROJECT_NAME} ${SOURCES})
