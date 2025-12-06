#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "AABB.h"

#include <vector>
#include <memory>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
private:
    aabb bbox;
public:
    std::vector<shared_ptr<hittable>> objects;
    
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { 
        objects.push_back(object); 
        bbox = aabb(bbox,object->bounding_box());
    }

    aabb bounding_box() const override {return bbox;}

    bool hit(const ray& r, interval t_range, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = t_range.max;

        for (const auto& object : objects) {
            if (object->hit(r, interval(t_range.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

};
#endif