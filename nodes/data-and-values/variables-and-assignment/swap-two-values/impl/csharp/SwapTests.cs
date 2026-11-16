// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

using Xunit;

public sealed class SwapTests
{
    public static TheoryData<string, long[], int, int, long[]> Cases => new()
    {
        { "first-and-last", new long[] { 1, 2, 3 }, 0, 2, new long[] { 3, 2, 1 } },
        { "middle-pair", new long[] { 10, 20, 30, 40 }, 1, 2, new long[] { 10, 30, 20, 40 } },
        { "two-items", new long[] { 5, 7 }, 0, 1, new long[] { 7, 5 } },
        { "single-item", new long[] { 9 }, 0, 0, new long[] { 9 } },
        { "same-index", new long[] { 1, 2, 3 }, 1, 1, new long[] { 1, 2, 3 } },
        { "higher-index-first", new long[] { 1, 2, 3, 4 }, 3, 0, new long[] { 4, 2, 3, 1 } },
        { "equal-values", new long[] { 4, 4 }, 0, 1, new long[] { 4, 4 } },
        { "negative-values", new long[] { -1, 0, 1 }, 0, 2, new long[] { 1, 0, -1 } },
        { "int32-extremes", new long[] { 2147483647, -2147483648 }, 0, 1, new long[] { -2147483648, 2147483647 } },
        { "eight-items", new long[] { 1, 2, 3, 4, 5, 6, 7, 8 }, 2, 6, new long[] { 1, 2, 7, 4, 5, 6, 3, 8 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void SwapItems(string name, long[] xs, int i, int j, long[] want)
    {
        Assert.NotNull(name);
        var before = (long[])xs.Clone();
        Assert.Equal(want, Swap.SwapItems(xs, i, j));
        Assert.Equal(before, xs);
    }
}
