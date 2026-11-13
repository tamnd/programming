// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

public static class Grid
{
    // Returns a jagged array: rows arrays of cols items, each item value.
    // new int[rows][] leaves every row null, so the loop makes each row.
    public static int[][] Make(int rows, int cols, int value)
    {
        var g = new int[rows][];
        for (var i = 0; i < rows; i++)
        {
            g[i] = new int[cols];
            Array.Fill(g[i], value);
        }
        return g;
    }
}
