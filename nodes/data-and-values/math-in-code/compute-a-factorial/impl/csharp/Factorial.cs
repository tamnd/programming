// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

using System;

/// <summary>The factorial of a non-negative integer.</summary>
public static class Factorial
{
    /// <summary>Returns n!. The checked multiplication throws OverflowException above 20!.</summary>
    public static long Compute(long n)
    {
        ArgumentOutOfRangeException.ThrowIfNegative(n);
        long f = 1;
        for (long i = 2; i <= n; i++)
        {
            f = checked(f * i);
        }
        return f;
    }
}
