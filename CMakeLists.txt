cmake_minimum_required(VERSION 3.28)
project(RayTracing1)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracing1 src/main.cpp
        include/vec3.h
        include/color.h
        include/ray.h
        include/hittable.h
        include/sphere.h
        include/rt_utils.h
        include/hittable_list.h
        include/camera.h
        include/interval.h
        include/stb_image_write.h
        include/material.h
)
