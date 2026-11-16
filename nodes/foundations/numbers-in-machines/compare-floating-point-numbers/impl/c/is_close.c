// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

#include "is_close.h"

#include <math.h>

bool is_close(double a, double b, double rel_tol, double abs_tol) {
    if (a == b) {
        return true; // also an infinity and itself
    }
    if (!isfinite(a) || !isfinite(b)) {
        return false; // NaN or an infinity
    }
    double diff = fabs(a - b);
    return diff <= fmax(rel_tol * fmax(fabs(a), fabs(b)), abs_tol);
}
