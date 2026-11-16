// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.
using System;

public static class FloatCompare
{
    // Tells whether a and b are close: |a - b| <= max(relTol * max(|a|, |b|), absTol).
    // NaN is close to no value, and an infinity is close only to itself.
    public static bool IsClose(double a, double b, double relTol, double absTol)
    {
        if (a == b)
        {
            return true; // also an infinity and itself
        }
        if (!double.IsFinite(a) || !double.IsFinite(b))
        {
            return false; // NaN or an infinity
        }
        double diff = Math.Abs(a - b);
        return diff <= Math.Max(relTol * Math.Max(Math.Abs(a), Math.Abs(b)), absTol);
    }
}
