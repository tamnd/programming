// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

using System;

/// <summary>A function that takes a function and returns a function.</summary>
public static class HigherOrder
{
    /// <summary>Returns a function that applies f to its argument times times.</summary>
    public static Func<int, int> Repeat(Func<int, int> f, int times)
    {
        return x =>
        {
            int value = x;
            for (int i = 0; i < times; i++)
            {
                value = f(value);
            }
            return value;
        };
    }

    /// <summary>Returns n / 2 for an even n and 3n + 1 for an odd n.</summary>
    public static int CollatzStep(int n)
    {
        return n % 2 == 0 ? n / 2 : 3 * n + 1;
    }

    /// <summary>Returns the value after times Collatz steps from start.</summary>
    public static int CollatzAfter(int start, int times)
    {
        return Repeat(CollatzStep, times)(start);
    }
}
