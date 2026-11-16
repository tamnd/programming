# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

import math


def to_cartesian(r: float, degrees: float) -> tuple[float, float]:
    """Return the point (x, y) at the distance r and the angle in degrees."""
    t = math.radians(degrees)
    return r * math.cos(t), r * math.sin(t)
