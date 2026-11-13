// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

using Xunit;

public sealed class CartesianProductTests
{
    public static TheoryData<string, int[][], int[][]> Cases => new()
    {
        { "two", new[] { new[] { 1, 2 }, new[] { 3, 4 } }, new[] { new[] { 1, 3 }, new[] { 1, 4 }, new[] { 2, 3 }, new[] { 2, 4 } } },
        { "three", new[] { new[] { 1, 2 }, new[] { 3 }, new[] { 4, 5 } }, new[] { new[] { 1, 3, 4 }, new[] { 1, 3, 5 }, new[] { 2, 3, 4 }, new[] { 2, 3, 5 } } },
        { "no-lists", new int[0][], new[] { new int[0] } },
        { "one-empty", new[] { new[] { 1, 2 }, new int[0] }, new int[0][] },
        { "single", new[] { new[] { 7, 8, 9 } }, new[] { new[] { 7 }, new[] { 8 }, new[] { 9 } } },
        { "duplicates", new[] { new[] { 1, 1 }, new[] { 2 } }, new[] { new[] { 1, 2 }, new[] { 1, 2 } } },
        { "negative", new[] { new[] { -1 }, new[] { -2, -3 } }, new[] { new[] { -1, -2 }, new[] { -1, -3 } } },
        { "extremes", new[] { new[] { int.MinValue, int.MaxValue }, new[] { 0 } }, new[] { new[] { int.MinValue, 0 }, new[] { int.MaxValue, 0 } } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[][] lists, int[][] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, CartesianProduct.Of(lists));
    }
}
