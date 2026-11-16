// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

#ifndef VEC2_HPP
#define VEC2_HPP

#include <cstdint>
#include <vector>

namespace vec2 {

struct Vec2 {
    std::int64_t x = 0;
    std::int64_t y = 0;

    // The compound operator is a member; operator+ below is built on it.
    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // The compiler writes == from the members, and != from ==.
    friend bool operator==(const Vec2&, const Vec2&) = default;
};

// A non-member, so that both operands are treated the same way.
inline Vec2 operator+(Vec2 a, const Vec2& b) { return a += b; }

// Returns the sum of the vectors (xs[i], ys[i]); throws std::invalid_argument when the lengths differ.
Vec2 vector_sum(const std::vector<std::int64_t>& xs, const std::vector<std::int64_t>& ys);

}  // namespace vec2

#endif
