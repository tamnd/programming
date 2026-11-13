// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

using Xunit;

public sealed class KeepOddTests
{
    public static TheoryData<string, int[], int[]> Cases => new()
    {
        { "mixed", new int[] { 1, 2, 3, 4, 5 }, new int[] { 1, 3, 5 } },
        { "tens", new int[] { 10, 15, 20, 25 }, new int[] { 15, 25 } },
        { "empty", new int[0], new int[0] },
        { "none-odd", new int[] { 2, 4, 6 }, new int[0] },
        { "all-odd", new int[] { 1, 3, 5 }, new int[] { 1, 3, 5 } },
        { "negative", new int[] { -3, -2, -1, 0 }, new int[] { -3, -1 } },
        { "zero", new int[] { 0 }, new int[0] },
        { "order", new int[] { 9, 1, 7 }, new int[] { 9, 1, 7 } },
        { "extremes", new int[] { -2147483648, 2147483647 }, new int[] { 2147483647 } },
        { "duplicates", new int[] { 3, 3, 2, 3 }, new int[] { 3, 3, 3 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, KeepOdd.Filter(items));
    }
}
