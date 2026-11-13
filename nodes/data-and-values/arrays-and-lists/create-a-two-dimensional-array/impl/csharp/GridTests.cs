// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

using Xunit;

public sealed class GridTests
{
    public static TheoryData<string, int, int, int, int[][]> Cases => new()
    {
        { "two-by-three", 2, 3, 0, new int[][] { new int[] { 0, 0, 0 }, new int[] { 0, 0, 0 } } },
        { "three-by-one", 3, 1, 7, new int[][] { new int[] { 7 }, new int[] { 7 }, new int[] { 7 } } },
        { "no-rows", 0, 3, 1, new int[0][] },
        { "no-cols", 2, 0, 1, new int[][] { new int[0], new int[0] } },
        { "one-cell", 1, 1, 5, new int[][] { new int[] { 5 } } },
        { "one-row", 1, 4, 2, new int[][] { new int[] { 2, 2, 2, 2 } } },
        { "one-col", 4, 1, 2, new int[][] { new int[] { 2 }, new int[] { 2 }, new int[] { 2 }, new int[] { 2 } } },
        { "negative", 2, 2, -3, new int[][] { new int[] { -3, -3 }, new int[] { -3, -3 } } },
        { "min-int", 2, 2, int.MinValue, new int[][] { new int[] { int.MinValue, int.MinValue }, new int[] { int.MinValue, int.MinValue } } },
        { "max-int", 1, 2, 2147483647, new int[][] { new int[] { 2147483647, 2147483647 } } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int rows, int cols, int value, int[][] want)
    {
        Assert.NotNull(name);
        var got = Grid.Make(rows, cols, value);
        Assert.Equal(want.Length, got.Length);
        for (var i = 0; i < want.Length; i++)
        {
            Assert.Equal(want[i], got[i]);
        }
        if (rows >= 2 && cols >= 1)
        {
            got[0][0] = value ^ 1;
            Assert.Equal(value, got[1][0]);
        }
    }
}
