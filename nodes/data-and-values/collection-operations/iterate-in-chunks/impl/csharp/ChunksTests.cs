// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

using System;
using Xunit;

public sealed class ChunksTests
{
    public static TheoryData<string, int[], int, int[][]> Cases => new()
    {
        { "pairs", new int[] { 1, 2, 3, 4, 5 }, 2, new int[][] { new[] { 1, 2 }, new[] { 3, 4 }, new[] { 5 } } },
        { "triples", new int[] { 1, 2, 3, 4, 5, 6 }, 3, new int[][] { new[] { 1, 2, 3 }, new[] { 4, 5, 6 } } },
        { "empty", new int[0], 3, new int[0][] },
        { "size-one", new int[] { 7, 8, 9 }, 1, new int[][] { new[] { 7 }, new[] { 8 }, new[] { 9 } } },
        { "size-larger", new int[] { 1, 2 }, 5, new int[][] { new[] { 1, 2 } } },
        { "size-equal", new int[] { 1, 2, 3 }, 3, new int[][] { new[] { 1, 2, 3 } } },
        { "last-shorter", new int[] { 1, 2, 3, 4 }, 3, new int[][] { new[] { 1, 2, 3 }, new[] { 4 } } },
        { "negative", new int[] { -1, -2, -3 }, 2, new int[][] { new[] { -1, -2 }, new[] { -3 } } },
        { "extremes", new int[] { -2147483648, 2147483647, 0 }, 2, new int[][] { new[] { -2147483648, 2147483647 }, new[] { 0 } } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int size, int[][] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Chunks.Split(items, size));
    }

    [Fact]
    public void SizeBelowOne()
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => Chunks.Split(new[] { 1 }, 0));
    }
}
