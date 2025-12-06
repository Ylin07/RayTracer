#ifndef COLOR_H
#define COLOR_H

#include "vec.h"
#include "interval.h"

using color = vec3;    // RGB color

inline void linear_to_gamma(color& c) {
    c[0] = std::sqrt(c[0]);
    c[1] = std::sqrt(c[1]);
    c[2] = std::sqrt(c[2]);
}

void write_color(std::ostream &out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    static const interval color_interval(0.0, 0.999);
    pixel_color[0] = color_interval.clamp(pixel_color[0]);
    pixel_color[1] = color_interval.clamp(pixel_color[1]);
    pixel_color[2] = color_interval.clamp(pixel_color[2]);
    linear_to_gamma(pixel_color);
    int ir = static_cast<int>(255.999 * pixel_color.x());
    int ig = static_cast<int>(255.999 * pixel_color.y());
    int ib = static_cast<int>(255.999 * pixel_color.z());

    out << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif