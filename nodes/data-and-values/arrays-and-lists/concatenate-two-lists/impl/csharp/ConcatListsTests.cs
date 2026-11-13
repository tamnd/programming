// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

using Xunit;

public sealed class ConcatListsTests
{
    public static TheoryData<string, int[], int[], int[]> Cases => new()
    {
        { "two-pairs", new int[] { 1, 2 }, new int[] { 3, 4 }, new int[] { 1, 2, 3, 4 } },
        { "uneven", new int[] { 5 }, new int[] { 6, 7, 8 }, new int[] { 5, 6, 7, 8 } },
        { "both-empty", new int[0], new int[0], new int[0] },
        { "left-empty", new int[0], new int[] { 1, 2 }, new int[] { 1, 2 } },
        { "right-empty", new int[] { 1, 2 }, new int[0], new int[] { 1, 2 } },
        { "duplicates", new int[] { 1, 1 }, new int[] { 1 }, new int[] { 1, 1, 1 } },
        { "order", new int[] { 3, 1 }, new int[] { 2 }, new int[] { 3, 1, 2 } },
        { "limit", new int[] { 2147483647 }, new int[] { int.MinValue }, new int[] { 2147483647, int.MinValue } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] a, int[] b, int[] want)
    {
        Assert.NotNull(name);
        var a0 = (int[])a.Clone();
        var b0 = (int[])b.Clone();
        Assert.Equal(want, ConcatLists.Of(a, b));
        Assert.Equal(a0, a);
        Assert.Equal(b0, b);
    }
}
