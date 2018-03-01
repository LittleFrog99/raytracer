cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

project(raytracer)

include_directories(/usr/local/include)
include_directories(src/headers)
include_directories(src)
set(SOURCES_CORE src/main.cpp src/core/ray.cpp src/core/geometry.cpp src/core/shade.cpp
    src/core/world.cpp src/core/viewplane.cpp src/core/tracer.cpp src/core/sampler.cpp)
set(SOURCES_GEOMETRY src/geometry/plane.cpp src/geometry/sphere.cpp src/geometry/function2d.cpp)
set(SOURCES_TRACER src/tracer/multipleobjects.cpp)
set(SOURCES_SAMPLER src/sampler/regular.cpp src/sampler/jittered.cpp src/sampler/purerandom.cpp
    src/sampler/nrooks.cpp src/sampler/multijittered.cpp)
set(SOURCES ${SOURCES_CORE} ${SOURCES_GEOMETRY} ${SOURCES_TRACER} ${SOURCES_SAMPLER})
add_executable(${PROJECT_NAME} ${SOURCES})
