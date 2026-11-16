// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

#include "perimeter.hpp"

#include <algorithm>

namespace {

// overloaded builds one visitor from several lambdas.
template <class... Fs> struct overloaded : Fs... {
    using Fs::operator()...;
};

} // namespace

std::optional<Shape> make_shape(std::string_view kind, const std::vector<int> &sides) {
    if (std::ranges::any_of(sides, [](int s) { return s <= 0; })) {
        return std::nullopt;
    }
    if (kind == "square" && sides.size() == 1) {
        return Square{sides[0]};
    }
    if (kind == "rectangle" && sides.size() == 2) {
        return Rectangle{sides[0], sides[1]};
    }
    if (kind == "triangle" && sides.size() == 3) {
        const int a = sides[0], b = sides[1], c = sides[2];
        if (a < b + c && b < a + c && c < a + b) {
            return Triangle{a, b, c};
        }
    }
    return std::nullopt;
}

// std::visit calls the lambda for the alternative that the variant holds. A
// missing alternative is a compile error.
int shape_perimeter(const Shape &shape) {
    return std::visit(overloaded{
                          [](const Square &s) { return 4 * s.side; },
                          [](const Rectangle &r) { return 2 * (r.width + r.height); },
                          [](const Triangle &t) { return t.a + t.b + t.c; },
                      },
                      shape);
}

std::optional<int> perimeter(std::string_view kind, const std::vector<int> &sides) {
    if (auto shape = make_shape(kind, sides)) {
        return shape_perimeter(*shape);
    }
    return std::nullopt;
}
