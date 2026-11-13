// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

using Xunit;

public sealed class ZipPairsTests
{
    public static TheoryData<string, int[], int[], (int, int)[]> Cases => new()
    {
        { "same-length", new int[] { 1, 2, 3 }, new int[] { 4, 5, 6 }, new (int, int)[] { (1, 4), (2, 5), (3, 6) } },
        { "tens", new int[] { 10, 20 }, new int[] { 30, 40 }, new (int, int)[] { (10, 30), (20, 40) } },
        { "empty", new int[0], new int[0], new (int, int)[0] },
        { "first-empty", new int[0], new int[] { 1, 2 }, new (int, int)[0] },
        { "second-empty", new int[] { 1, 2 }, new int[0], new (int, int)[0] },
        { "first-shorter", new int[] { 1 }, new int[] { 7, 8 }, new (int, int)[] { (1, 7) } },
        { "second-shorter", new int[] { 1, 2, 3 }, new int[] { 9 }, new (int, int)[] { (1, 9) } },
        { "negative", new int[] { -1, -2 }, new int[] { 3, -4 }, new (int, int)[] { (-1, 3), (-2, -4) } },
        { "extremes", new int[] { -2147483648, 2147483647 }, new int[] { 2147483647, -2147483648 }, new (int, int)[] { (-2147483648, 2147483647), (2147483647, -2147483648) } },
        { "duplicates", new int[] { 5, 5 }, new int[] { 5, 5 }, new (int, int)[] { (5, 5), (5, 5) } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] a, int[] b, (int, int)[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, ZipPairs.Pairs(a, b));
    }
}
