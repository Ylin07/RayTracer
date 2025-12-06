#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec.h"

class sphere: public hittable {
public:
    // 静态球体
    sphere(const point3& static_center, double radius, shared_ptr<material> mat)
        : center(static_center,vec3(0,0,0)), radius(radius), mat(mat) {
            auto rvec = vec3(radius,radius,radius);
            bbox = aabb(static_center - rvec, static_center + rvec);
        }
    // 动态球体
    sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat)
        : center(center1,center2-center1), radius(radius), mat(mat) {
            auto rvec = vec3(radius,radius,radius);
            aabb box1(center.at(0) - rvec, center.at(0) + rvec);
            aabb box2(center.at(1) - rvec, center.at(1) + rvec);
            bbox = aabb(box1,box2);
        }

    aabb bounding_box() const override {return bbox;}

    bool hit(const ray& r, interval t_range, hit_record& rec) const override {
        point3 cur_center = center.at(r.time());
        vec3 oc = r.origin() - cur_center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;
        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) {
            return false;
        }
        auto sqrtd = std::sqrt(discriminant);
        auto root = (-half_b - sqrtd) / a;
        if (!t_range.contains(root)) {
            root = (-half_b + sqrtd) / a;
            if (!t_range.contains(root)) {
                return false;
            }
        }
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - cur_center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_uv(outward_normal,rec.u,rec.v);
        rec.mat = mat;
        return true;
    }

    static void get_uv(const point3& p, double& u, double& v){
        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(),p.x()) + pi;
        u = phi / (2*pi);
        v = theta / pi;
    }

private:
    ray center;
    double radius;
    shared_ptr<material> mat;
    aabb bbox;
};

#endif