// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

using Xunit;

public sealed class FlattenTests
{
    public static TheoryData<string, int[][], int[]> Cases => new()
    {
        { "three", new int[][] { new int[] { 1, 2 }, new int[] { 3 }, new int[] { 4, 5, 6 } }, new int[] { 1, 2, 3, 4, 5, 6 } },
        { "two", new int[][] { new int[] { 7 }, new int[] { 8, 9 } }, new int[] { 7, 8, 9 } },
        { "empty", new int[0][], new int[0] },
        { "empty-inner", new int[][] { new int[0], new int[] { 1 }, new int[0] }, new int[] { 1 } },
        { "all-empty", new int[][] { new int[0], new int[0] }, new int[0] },
        { "single", new int[][] { new int[] { 5 } }, new int[] { 5 } },
        { "one-list", new int[][] { new int[] { 1, 2, 3 } }, new int[] { 1, 2, 3 } },
        { "negative", new int[][] { new int[] { -1, -2 }, new int[] { 3 } }, new int[] { -1, -2, 3 } },
        { "extremes", new int[][] { new int[] { int.MinValue }, new int[] { 2147483647, 0 } }, new int[] { int.MinValue, 2147483647, 0 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[][] lists, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Flatten.Join(lists));
    }
}
