// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

using System.Linq;

public static class KeepOdd
{
    // ToArray runs the query now. Without it, Where runs each time the
    // result is read. Use != 0, not == 1: the remainder of a negative item can be negative.
    public static int[] Filter(int[] items) => items.Where(x => x % 2 != 0).ToArray();
}
