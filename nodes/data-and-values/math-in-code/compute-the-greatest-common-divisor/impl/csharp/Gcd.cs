// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

using System;

/// <summary>The greatest common divisor and the least common multiple of two long values.</summary>
public static class Gcd
{
    /// <summary>Returns the GCD and the LCM. Math.Abs throws OverflowException for long.MinValue.</summary>
    public static (long Gcd, long Lcm) GcdAndLcm(long a, long b)
    {
        long x = Math.Abs(a);
        long y = Math.Abs(b);
        while (y != 0)
        {
            (x, y) = (y, x % y);
        }
        long lcm = a == 0 || b == 0 ? 0 : checked(Math.Abs(a) / x * Math.Abs(b));
        return (x, lcm);
    }
}
