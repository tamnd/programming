// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

using Xunit;

public sealed class CombinationsTests
{
    public static TheoryData<string, int[], int, int[][]> Cases => new()
    {
        { "two-of-four", new[] { 1, 2, 3, 4 }, 2, new[] { new[] { 1, 2 }, new[] { 1, 3 }, new[] { 1, 4 }, new[] { 2, 3 }, new[] { 2, 4 }, new[] { 3, 4 } } },
        { "three-of-four", new[] { 1, 2, 3, 4 }, 3, new[] { new[] { 1, 2, 3 }, new[] { 1, 2, 4 }, new[] { 1, 3, 4 }, new[] { 2, 3, 4 } } },
        { "zero-k", new[] { 1, 2 }, 0, new[] { new int[0] } },
        { "empty", new int[0], 0, new[] { new int[0] } },
        { "k-too-large", new[] { 1, 2 }, 3, new int[0][] },
        { "all", new[] { 1, 2, 3 }, 3, new[] { new[] { 1, 2, 3 } } },
        { "duplicates", new[] { 1, 1, 2 }, 2, new[] { new[] { 1, 1 }, new[] { 1, 2 }, new[] { 1, 2 } } },
        { "unsorted", new[] { 3, 1, 2 }, 2, new[] { new[] { 3, 1 }, new[] { 3, 2 }, new[] { 1, 2 } } },
        { "extremes", new[] { int.MinValue, int.MaxValue }, 1, new[] { new[] { int.MinValue }, new[] { int.MaxValue } } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int k, int[][] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Combinations.Of(items, k));
    }
}
