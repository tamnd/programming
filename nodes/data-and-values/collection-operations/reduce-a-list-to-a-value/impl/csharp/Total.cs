// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

using System.Linq;

public static class Total
{
    // The seed 0L gives a long accumulator, and the result for an empty array.
    public static int Of(int[] items) => checked((int)items.Aggregate(0L, (acc, x) => acc + x));
}
