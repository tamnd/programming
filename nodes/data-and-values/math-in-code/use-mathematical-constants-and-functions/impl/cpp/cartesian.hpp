// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

#ifndef TO_CARTESIAN_HPP
#define TO_CARTESIAN_HPP

#include <cmath>
#include <numbers>
#include <utility>

// Returns the point (x, y) at the distance r and the angle in degrees.
inline std::pair<double, double> to_cartesian(double r, double degrees) {
    const double t = degrees * std::numbers::pi / 180.0;
    return {r * std::cos(t), r * std::sin(t)};
}

#endif
