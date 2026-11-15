// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

using Xunit;

public sealed class FindInGridTests
{
    public static TheoryData<string, int[][], int, int, int> Cases => new()
    {
        { "middle", new[] { new[] { 1, 2, 3 }, new[] { 4, 5, 6 }, new[] { 7, 8, 9 } }, 5, 1, 1 },
        { "first-twenty", new[] { new[] { 3, 8, 12 }, new[] { 5, 20, 20 }, new[] { 20, 1, 4 } }, 20, 1, 1 },
        { "first-cell", new[] { new[] { 9, 1 }, new[] { 2, 3 } }, 9, 0, 0 },
        { "last-cell", new[] { new[] { 1, 2 }, new[] { 3, 9 } }, 9, 1, 1 },
        { "row-major", new[] { new[] { 1, 2, 9 }, new[] { 9, 3, 4 } }, 9, 0, 2 },
        { "missing", new[] { new[] { 1, 2 }, new[] { 3, 4 } }, 9, -1, -1 },
        { "empty-grid", new int[0][], 1, -1, -1 },
        { "empty-rows", new[] { new int[0], new int[0], new[] { 5 } }, 5, 2, 0 },
        { "ragged", new[] { new[] { 1 }, new[] { 2, 3, 4 }, new[] { 5, 6 } }, 6, 2, 1 },
        { "negative", new[] { new[] { -1, -2 }, new[] { -3, -4 } }, -3, 1, 0 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[][] grid, int target, int row, int col)
    {
        Assert.NotNull(name);
        Assert.Equal((row, col), FindInGrid.Of(grid, target));
    }
}
