// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

public static class FindInGrid
{
    // Returns (row, column) of the first cell that equals target, or (-1, -1).
    public static (int Row, int Col) Of(int[][] grid, int target)
    {
        for (int r = 0; r < grid.Length; r++)
        {
            for (int c = 0; c < grid[r].Length; c++)
            {
                if (grid[r][c] == target)
                {
                    // return leaves both loops. break would leave only the inner one.
                    return (r, c);
                }
            }
        }
        return (-1, -1);
    }
}
