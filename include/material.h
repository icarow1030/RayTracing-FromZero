#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt_utils.h"
#include "hittable.h"
#include "color.h"

// Classe base abstrata
class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
            const ray& r_in, const hit_record&, color& attenuation, ray& scattered
            ) const = 0;
};

// Material Fosco / Difuso
class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_in_unit_sphere();

        if(scatter_direction.length_squared() < 1e-8) {
            scatter_direction = rec.normal;
        }

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

// Material Metálico
class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);

        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;

        return (dot(scattered.direction(), rec.normal) > 0);
    }
private:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double index_of_refraction) : index_of_refraction(index_of_refraction) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        attenuation = color(1.0, 1.0, 1.0); // o vidro puro não absorve cor, então ele sempre vai ter 100% de albedo

        // Se bateu por fora, é ar para vidro. Se bateu por dentro, é vidro para ar.
        // respectivamente: 1.0/ir  e   ir.
        double refraction_ratio = rec.front_face ? (1.0/index_of_refraction) : index_of_refraction;

        vec3 unit_direction = unit_vector(r_in.direction());

        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        // verifica se a refração é impossível (reflexão interna total)
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            // a luz não consegue sair ou reflete pelo ângulo (Schlick) ->  Vira Espelho
            direction = reflect(unit_direction, rec.normal);
        } else {
            // a luz atravessa o vidro - refração normal
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = ray(rec.p, direction);
        return true;
    }
private:
    double index_of_refraction;

    static double reflectance(double cosine, double ref_index) {
        // Equação de Schlick
        auto r0 = (1 - ref_index) / (1 + ref_index);
        r0 = r0*r0;
        return r0 + (1 - r0)*std::pow((1-cosine), 5);
    }
};

#endif //MATERIAL_H
