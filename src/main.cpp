#include "utils.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "BVH.h"


void bounding_ball(){
    // 世界场景
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.5, color(0,0,0), color(1,1,1));
    auto ground_material = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -8; a < 8; a++) {
        for (int b = -8; b < 8; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.5) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,0.3), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.8) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.2);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    auto center2 = center + vec3(0, random_double(0,0), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    auto center2 = center + vec3(0, random_double(0,0.0), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    
    // BVH加速
    world = hittable_list(make_shared<bvh_node>(world));

    // 相机
    camera cam;
    cam.aspect_ratio = 25.0 / 16.0;
    cam.image_width = 1000;
    cam.samples_per_pixel = 50;
    cam.max_depth = 20;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_distance = 10;

    cam.render(world);
}



void checkered_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio      = 25.0 / 16.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}


void earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 25.0 / 16.0;
    cam.image_width       = 2000;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 10;

    cam.vfov     = 20;
    cam.lookfrom = point3(-3.75,4,-9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

int main(){
    switch(3){
        case 1: bounding_ball(); break;
        case 2: checkered_spheres(); break;
        case 3: earth(); break;
    }
}