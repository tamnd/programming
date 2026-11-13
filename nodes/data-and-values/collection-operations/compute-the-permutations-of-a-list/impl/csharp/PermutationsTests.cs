// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

using Xunit;

public sealed class PermutationsTests
{
    public static TheoryData<string, int[], int[][]> Cases => new()
    {
        { "three", new[] { 1, 2, 3 }, new[] { new[] { 1, 2, 3 }, new[] { 1, 3, 2 }, new[] { 2, 1, 3 }, new[] { 2, 3, 1 }, new[] { 3, 1, 2 }, new[] { 3, 2, 1 } } },
        { "two", new[] { 5, 6 }, new[] { new[] { 5, 6 }, new[] { 6, 5 } } },
        { "empty", new int[0], new[] { new int[0] } },
        { "single", new[] { 7 }, new[] { new[] { 7 } } },
        { "duplicates", new[] { 1, 1, 2 }, new[] { new[] { 1, 1, 2 }, new[] { 1, 2, 1 }, new[] { 1, 1, 2 }, new[] { 1, 2, 1 }, new[] { 2, 1, 1 }, new[] { 2, 1, 1 } } },
        { "unsorted", new[] { 3, 1, 2 }, new[] { new[] { 3, 1, 2 }, new[] { 3, 2, 1 }, new[] { 1, 3, 2 }, new[] { 1, 2, 3 }, new[] { 2, 3, 1 }, new[] { 2, 1, 3 } } },
        { "extremes", new[] { int.MinValue, int.MaxValue }, new[] { new[] { int.MinValue, int.MaxValue }, new[] { int.MaxValue, int.MinValue } } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int[][] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Permutations.Of(items));
    }
}
