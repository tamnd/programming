// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

using System.Linq;

public static class Flatten
{
    // Join returns the items of the inner arrays, one level deep, in order.
    public static int[] Join(int[][] lists)
    {
        return lists.SelectMany(xs => xs).ToArray();
    }
}
