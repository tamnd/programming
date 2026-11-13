// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

using Xunit;

public sealed class SliceListTests
{
    public static TheoryData<string, int[], int, int, int[]> Cases => new()
    {
        { "middle", new int[] { 1, 2, 3, 4, 5 }, 1, 3, new int[] { 2, 3 } },
        { "prefix", new int[] { 1, 2, 3 }, 0, 2, new int[] { 1, 2 } },
        { "suffix", new int[] { 1, 2, 3 }, 1, 3, new int[] { 2, 3 } },
        { "empty-list", new int[0], 0, 0, new int[0] },
        { "empty-slice", new int[] { 1, 2, 3 }, 2, 2, new int[0] },
        { "whole", new int[] { 1, 2, 3 }, 0, 3, new int[] { 1, 2, 3 } },
        { "single", new int[] { 7 }, 0, 1, new int[] { 7 } },
        { "last-one", new int[] { 1, 2, 3 }, 2, 3, new int[] { 3 } },
        { "at-end", new int[] { 1, 2, 3 }, 3, 3, new int[0] },
        { "limit", new int[] { 2147483647, int.MinValue, 0 }, 0, 2, new int[] { 2147483647, int.MinValue } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int start, int stop, int[] want)
    {
        Assert.NotNull(name);
        var items0 = (int[])items.Clone();
        var got = SliceList.Of(items, start, stop);
        Assert.Equal(want, got);
        if (got.Length > 0)
        {
            got[0] ^= 1;
        }
        Assert.Equal(items0, items);
    }

    [Fact]
    public void BadRange()
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => SliceList.Of([1, 2, 3], 0, 4));
        Assert.Throws<ArgumentOutOfRangeException>(() => SliceList.Of([1, 2, 3], 2, 1));
    }
}
