// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

using Xunit;

public sealed class SquareAllTests
{
    public static TheoryData<string, int[], int[]> Cases => new()
    {
        { "small", new int[] { 1, 2, 3 }, new int[] { 1, 4, 9 } },
        { "tens", new int[] { 5, 10 }, new int[] { 25, 100 } },
        { "empty", new int[0], new int[0] },
        { "single", new int[] { 7 }, new int[] { 49 } },
        { "negative", new int[] { -3, -1 }, new int[] { 9, 1 } },
        { "zero", new int[] { 0 }, new int[] { 0 } },
        { "limit", new int[] { 46340, -46340 }, new int[] { 2147395600, 2147395600 } },
        { "order", new int[] { 3, 1, 2 }, new int[] { 9, 1, 4 } },
        { "duplicates", new int[] { 2, 2 }, new int[] { 4, 4 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, SquareAll.Map(items));
    }
}
