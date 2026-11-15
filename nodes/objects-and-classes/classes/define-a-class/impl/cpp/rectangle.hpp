// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

#pragma once

#include <string>
#include <vector>

// A rectangle with a width and a height.
class Rectangle {
public:
    Rectangle(int width, int height) : width_(width), height_(height) {}

    // const after the parameter list: the method does not change the object.
    int area() const { return width_ * height_; }
    int perimeter() const { return 2 * (width_ + height_); }

    // Multiplies both sides by factor. This changes the object.
    void scale(int factor);

    std::string to_string() const;

private:
    int width_;
    int height_;
};

// rectangle_report returns the area and the perimeter, before and after
// scale(factor).
std::vector<int> rectangle_report(int width, int height, int factor);
