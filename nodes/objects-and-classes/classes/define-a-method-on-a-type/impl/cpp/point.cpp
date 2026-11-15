// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

#include "point.hpp"

#include <cstdlib>

Point Point::translate(int dx, int dy) const {
    return Point{x + dx, y + dy};
}

int Point::manhattan(const Point &other) const {
    return std::abs(x - other.x) + std::abs(y - other.y);
}

std::vector<int> point_report(int x, int y, int dx, int dy) {
    const Point origin{0, 0};
    const Point p{x, y};
    const Point q = p.translate(dx, dy);
    return {q.x, q.y, p.manhattan(q), q.manhattan(origin)};
}
