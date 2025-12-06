#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include "utils.h"

class camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    int max_depth = 10;
    double vfov = 90.0; // 相机在垂直方向上的视野角度

    double defocus_angle = 0.0;
    double focus_distance = 1.0;

    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 vup = point3(0, 1, 0);

    void render(const hittable& world) {
        initialize_camera();

        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color*pixel_sample_scale);    
            }
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::clog << "\nDone.\n" <<  "Time: " << std::chrono::duration<double>(end - start).count() << " s\n";
    }

private:
    int image_height;
    double pixel_sample_scale;
    point3 camera_origin;
    point3 pixel_origin;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u,v,w;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    void initialize_camera(){
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pixel_sample_scale = 1.0 / samples_per_pixel;
        camera_origin = lookfrom;
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h * focus_distance;
        auto viewport_width = (double(image_width) / image_height) * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // 从相机位置计算视口左上角的位置
        auto viewport_top_left = camera_origin - viewport_u / 2 - viewport_v / 2 - (focus_distance * w);
        // 左上角第一个像素的中心位置
        pixel_origin = viewport_top_left + pixel_delta_u / 2 + pixel_delta_v / 2;
    
        auto defocus_radius = focus_distance * tan(degrees_to_radians(defocus_angle) / 2);
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;
    }

    ray get_ray(int i, int j){
        auto offset = sample_pixel_offset();
        auto pixel_sample = pixel_origin + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;
        auto ray_origin = (defocus_angle > 0.0) ? defocus_disk_sample() : camera_origin;
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();
        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 sample_pixel_offset() {
        return vec3(random_double()-0.5, random_double()-0.5, 0);
    }

    point3 defocus_disk_sample(){
        auto p = random_in_unit_disk();
        return camera_origin + p.x() * defocus_disk_u + p.y() * defocus_disk_v;
    }

    color ray_color(const ray& r, int depth, const hittable& world) {

        if (depth <= 0) return color(0, 0, 0);

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            return color(0, 0, 0);
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); 
    }
};

#endif