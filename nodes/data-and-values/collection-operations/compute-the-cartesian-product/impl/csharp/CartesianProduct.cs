// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

using System;
using System.Collections.Generic;
using System.Linq;

public static class CartesianProduct
{
    // Of returns each tuple that takes one item from each array, in nested-loop order.
    public static int[][] Of(int[][] lists)
    {
        IEnumerable<int[]> tuples = new[] { Array.Empty<int>() };
        foreach (int[] xs in lists)
        {
            // Each tuple t gives one new tuple for each item x of xs.
            tuples = tuples.SelectMany(t => xs, (t, x) => t.Append(x).ToArray());
        }
        return tuples.ToArray();
    }
}
