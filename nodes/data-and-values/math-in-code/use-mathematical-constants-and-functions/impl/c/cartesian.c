// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

#include "cartesian.h"

#include <math.h>

// Standard C has no constant for pi; M_PI is POSIX. acos(-1) is pi.
void to_cartesian(double r, double degrees, double *x, double *y) {
    const double pi = acos(-1.0);
    double t = degrees * pi / 180.0;
    *x = r * cos(t);
    *y = r * sin(t);
}
