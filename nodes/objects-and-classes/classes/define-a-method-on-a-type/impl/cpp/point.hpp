// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

#ifndef POINT_HPP
#define POINT_HPP

#include <vector>

// A point on a grid. A struct is a class whose members are public by default,
// and it can have member functions like any class.
struct Point {
    int x;
    int y;

    // Returns the point moved by (dx, dy). const: it does not change *this.
    Point translate(int dx, int dy) const;

    // Returns the grid distance |dx| + |dy| to another point.
    int manhattan(const Point &other) const;

    bool operator==(const Point &other) const = default;
};

std::vector<int> point_report(int x, int y, int dx, int dy);

#endif
