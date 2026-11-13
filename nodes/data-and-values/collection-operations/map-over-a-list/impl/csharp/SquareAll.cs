// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

using System.Linq;

public static class SquareAll
{
    // ToArray runs the query now. Without it, Select runs each time the
    // result is read.
    public static int[] Map(int[] items) => items.Select(x => x * x).ToArray();
}
