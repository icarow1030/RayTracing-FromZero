cmake_minimum_required(VERSION 3.28)
project(RayTracing1)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracing1 main.cpp
        vec3.h
        color.h
        ray.h
        hittable.h
        sphere.h
        rt_utils.h
)
