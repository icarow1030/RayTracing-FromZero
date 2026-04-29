#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "rt_utils.h"
#include <vector>
#include <algorithm>

class camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 50;
    double vfov = 90.0;
    point3 lookfrom = point3(0,0,-1);
    point3 lookat = point3(0,0,0);
    vec3 vup = vec3(0,1,0);

    void render(const hittable& world) {
        initialize();

        // *3 -> RGB (3 canais)
        std::vector<unsigned char> image_data(image_width * image_height * 3);
        int pixel_index = 0;

        for(int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for(int i = 0; i < image_width; i++) {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - center;
                ray r(center, ray_direction);

                color pixel_color(0,0,0);
                for(int sample = 0; sample < samples_per_pixel; sample++) {
                    r = get_ray(i, j);
                    pixel_color += ray_color(r, world);
                }

                image_data[pixel_index++] = static_cast<unsigned char>(256 * std::clamp(pixel_color.x() * pixel_samples_scale, 0.0, 0.999));
                image_data[pixel_index++] = static_cast<unsigned char>(256 * std::clamp(pixel_color.y() * pixel_samples_scale, 0.0, 0.999));
                image_data[pixel_index++] = static_cast<unsigned char>(256 * std::clamp(pixel_color.z() * pixel_samples_scale, 0.0, 0.999));
            }
        }
        std::clog << "\rDone rendering. Salvando imagem...\n";

        stbi_write_png("C:/Users/icaro/CLionProjects/RayTracing1/images/output.png", image_width, image_height, 3, image_data.data(), image_width * 3);
        std::clog << "Imagem salva como output.png!\n";
    }

private:
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double pixel_samples_scale;
    vec3 u, v, w;

    ray get_ray(int i, int j) const {
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        auto pixel_sample = pixel00_loc + ((i + px) * pixel_delta_u) + ((j + py) * pixel_delta_v);
        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;
        return ray(ray_origin, ray_direction);
    }

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        // A câmera agora fica exatamente onde eu definir em lookfrom
        center = lookfrom;

        // Geometria da lente (fov - field of view)
        focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        viewport_height = 2.0 * h * focal_length;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Sistema de Coordenadas Ortonormais da Câmera
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray& r, const hittable& world) const {
        hit_record rec;
        if(world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1,1,1));
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
    }
};

#endif // CAMERA_H
