# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compare-floating-point-numbers/.
import math


def is_close(a: float, b: float, rel_tol: float, abs_tol: float) -> bool:
    """Tells whether a and b are close: a relative tolerance for large values,
    an absolute tolerance near zero. NaN is close to no value, and an
    infinity is close only to itself."""
    return math.isclose(a, b, rel_tol=rel_tol, abs_tol=abs_tol)
