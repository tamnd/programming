// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

using System;

public static class SortInts
{
    // A copy, so items does not change. Array.Sort sorts in place.
    public static int[] Sort(int[] items)
    {
        var output = (int[])items.Clone();
        Array.Sort(output);
        return output;
    }
}
