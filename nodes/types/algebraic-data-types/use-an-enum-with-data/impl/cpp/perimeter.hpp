// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

#pragma once

#include <optional>
#include <string_view>
#include <variant>
#include <vector>

struct Square {
    int side;
    bool operator==(const Square &) const = default;
};

struct Rectangle {
    int width, height;
    bool operator==(const Rectangle &) const = default;
};

struct Triangle {
    int a, b, c;
    bool operator==(const Triangle &) const = default;
};

// A std::variant holds exactly one of its alternatives and knows which one.
using Shape = std::variant<Square, Rectangle, Triangle>;

std::optional<Shape> make_shape(std::string_view kind, const std::vector<int> &sides);

int shape_perimeter(const Shape &shape);

std::optional<int> perimeter(std::string_view kind, const std::vector<int> &sides);
