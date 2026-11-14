// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

using Xunit;

public sealed class IndexOfTests
{
    public static TheoryData<string, int[], int, int> Cases => new()
    {
        { "middle", new int[] { 4, 7, 1 }, 7, 1 },
        { "absent", new int[] { 4, 7, 1 }, 9, -1 },
        { "empty", new int[0], 0, -1 },
        { "single", new int[] { 5 }, 5, 0 },
        { "single-absent", new int[] { 5 }, 0, -1 },
        { "first", new int[] { 7, 8, 9 }, 7, 0 },
        { "last", new int[] { 7, 8, 9 }, 9, 2 },
        { "duplicates", new int[] { 3, 5, 3, 5 }, 5, 1 },
        { "all-same", new int[] { 2, 2, 2 }, 2, 0 },
        { "negative", new int[] { 0, -1, -2 }, -2, 2 },
        { "limit", new int[] { 2147483647, int.MinValue }, int.MinValue, 1 },
        { "limit-absent", new int[] { 2147483647 }, int.MinValue, -1 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int value, int want)
    {
        Assert.NotNull(name);
        var items0 = (int[])items.Clone();
        Assert.Equal(want, IndexOf.Of(items, value));
        Assert.Equal(items0, items);
    }
}
