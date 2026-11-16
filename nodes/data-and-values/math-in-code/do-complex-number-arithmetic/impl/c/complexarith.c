// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

#include "complexarith.h"

#include <complex.h>

// The operator / on double complex does not overflow in intermediate results.
void complex_arithmetic(const double a[2], const double b[2], double out[8]) {
    double complex z = CMPLX(a[0], a[1]);
    double complex w = CMPLX(b[0], b[1]);
    double complex s = z + w, p = z * w, q = z / w;
    out[0] = creal(s);
    out[1] = cimag(s);
    out[2] = creal(p);
    out[3] = cimag(p);
    out[4] = creal(q);
    out[5] = cimag(q);
    out[6] = cabs(z);
    out[7] = carg(z);
}
