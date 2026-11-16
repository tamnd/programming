// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.
using System;

public static class Rounding
{
    // Rounds x to the nearest integer; a half goes away from zero.
    // Math.Round(x) without a mode rounds a half to the even integer.
    public static long RoundHalfAway(double x)
    {
        return (long)Math.Round(x, MidpointRounding.AwayFromZero);
    }
}
