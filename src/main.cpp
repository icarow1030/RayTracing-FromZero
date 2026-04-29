#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"
#include "../include/hittable_list.h"
#include "../include/sphere.h"
#include "../include/camera.h"
#include "../include/rt_utils.h"

#include <iostream>

using std::make_shared;

int main() {
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5,-1), 100));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(0,0,3);
    cam.lookat = point3(0,0,-1);
    cam.vup = vec3(0,1,0);

    cam.render(world);
    return 0;

}