// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

using System.Collections.Generic;

public static class PartitionOdd
{
    // Split returns the odd items and the even items, each in input order.
    public static (int[] Odd, int[] Even) Split(int[] items)
    {
        var odd = new List<int>();
        var even = new List<int>();
        foreach (int x in items)
        {
            // Use != 0, not == 1: the remainder of a negative item can be negative.
            (x % 2 != 0 ? odd : even).Add(x);
        }
        return (odd.ToArray(), even.ToArray());
    }
}
