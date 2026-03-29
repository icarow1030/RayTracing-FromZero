#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin(); // 0,0,-1
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = h*h - a*c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        // retorna o t da primeira intersecção
        return (h - sqrt(discriminant)) / a;
    }
}

// is going to return the color for the ray
// black for now
color ray_color(const ray& r) {
    point3 sphere_center = point3(0,0,-1);
    auto t = hit_sphere(sphere_center, 0.5, r);
    if(t >= 0.0) {
        point3 p = r.origin() + t * r.direction();
        point3 normal = unit_vector(p - sphere_center);
        double product = dot(normal, unit_vector(-r.direction()));
        double intensity = (product > 0.0) ? product : 0.0;
        return intensity * color(1, 0, 0);
    }

    if(t >= 0.0) {
        point3 p = r.at(t);
        vec3 normal = unit_vector(p - sphere_center);

        // 1. Defina a posição da sua luz no mundo
        point3 light_pos = point3(0, 0, 0); // Luz vinda da direita e de cima

        // 2. Calcule a direção da luz (do ponto P para a luz)
        // É fundamental normalizar para o dot product ser o cosseno
        vec3 light_dir = unit_vector(light_pos - p);

        // 3. O produto escalar nos dá a intensidade baseada no ângulo
        double product = dot(normal, light_dir);
        double intensity = (product > 0.0) ? product : 0.0;

        // Retorna a cor da esfera multiplicada pela intensidade da luz
        return intensity * color(1, 0, 0);
    }

    // 1. Normalizamos a direção (fundamental para o gradiente pq a cor é 0 a 1)
    vec3 unit_direction = unit_vector(r.direction());

    // 2. Mapeamos o y para 0 a 1
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);

}

int main() {
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    // Ajustando de acordo com o cálculo final da imagem
    // para se adaptar
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = point3(0,0,0);

    // Calculate the vectors across the horizontal and down the vertical viewport edge
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2-viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for(int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for(int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone\n";
}