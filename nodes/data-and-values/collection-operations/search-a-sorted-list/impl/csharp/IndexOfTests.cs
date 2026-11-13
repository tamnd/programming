// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

using Xunit;

public sealed class IndexOfTests
{
    public static TheoryData<string, int[], int, int> Cases => new()
    {
        { "middle", new int[] { 1, 3, 5, 7, 9 }, 5, 2 },
        { "first", new int[] { 1, 3, 5, 7, 9 }, 1, 0 },
        { "last", new int[] { 1, 3, 5, 7, 9 }, 9, 4 },
        { "missing", new int[] { 1, 3, 5, 7, 9 }, 4, -1 },
        { "below", new int[] { 1, 3, 5 }, 0, -1 },
        { "above", new int[] { 1, 3, 5 }, 6, -1 },
        { "empty", new int[0], 1, -1 },
        { "single-hit", new int[] { 7 }, 7, 0 },
        { "single-miss", new int[] { 7 }, 8, -1 },
        { "duplicates", new int[] { 1, 2, 2, 2, 3 }, 2, 1 },
        { "all-same", new int[] { 4, 4, 4, 4 }, 4, 0 },
        { "negative", new int[] { -9, -5, -1, 0 }, -5, 1 },
        { "extremes", new int[] { int.MinValue, 0, 2147483647 }, 2147483647, 2 },
        { "min-target", new int[] { int.MinValue, 0, 2147483647 }, int.MinValue, 0 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int target, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, IndexOf.Find(items, target));
    }
}
