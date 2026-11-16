// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

using System;

/// <summary>Fibonacci numbers, with F(0) = 0 and F(1) = 1.</summary>
public static class Fibonacci
{
    /// <summary>Returns F(n). The checked addition throws OverflowException above F(92).</summary>
    public static long Compute(long n)
    {
        ArgumentOutOfRangeException.ThrowIfNegative(n);
        if (n == 0)
        {
            return 0;
        }
        long a = 0;
        long b = 1;
        for (long i = 1; i < n; i++)
        {
            (a, b) = (b, checked(a + b));
        }
        return b;
    }
}
