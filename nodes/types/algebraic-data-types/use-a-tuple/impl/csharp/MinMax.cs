// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

using System.Collections.Generic;

public static class MinMax
{
    // Returns a value tuple with named elements. The names help the reader;
    // the type is (int, int).
    public static (int Min, int Max) Of(IReadOnlyList<int> numbers)
    {
        int lo = numbers[0], hi = numbers[0];
        foreach (int n in numbers)
        {
            if (n < lo) lo = n;
            if (n > hi) hi = n;
        }
        return (lo, hi);
    }
}
