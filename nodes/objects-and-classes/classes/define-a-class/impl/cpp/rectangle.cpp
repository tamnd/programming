// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

#include "rectangle.hpp"

void Rectangle::scale(int factor) {
    width_ *= factor;
    height_ *= factor;
}

std::string Rectangle::to_string() const {
    return "Rectangle(" + std::to_string(width_) + ", " + std::to_string(height_) + ")";
}

std::vector<int> rectangle_report(int width, int height, int factor) {
    Rectangle r(width, height);
    std::vector<int> out{r.area(), r.perimeter()};
    r.scale(factor);
    out.push_back(r.area());
    out.push_back(r.perimeter());
    return out;
}
