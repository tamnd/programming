# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

import cmath


def complex_arithmetic(a: list[float], b: list[float]) -> list[float]:
    """Return the sum, the product and the quotient of a and b, then abs(a) and phase(a).

    a and b are pairs [re, im]. Each complex result gives two numbers, the
    real part and the imaginary part. b must not be 0.
    """
    z, w = complex(*a), complex(*b)
    s, p, q = z + w, z * w, z / w
    return [s.real, s.imag, p.real, p.imag, q.real, q.imag, abs(z), cmath.phase(z)]
