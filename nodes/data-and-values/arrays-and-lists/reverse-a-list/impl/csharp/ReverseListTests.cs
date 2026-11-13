// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

using Xunit;

public sealed class ReverseListTests
{
    public static TheoryData<string, int[], int[]> Cases => new()
    {
        { "three", new int[] { 1, 2, 3 }, new int[] { 3, 2, 1 } },
        { "four", new int[] { 4, 7, 1, 9 }, new int[] { 9, 1, 7, 4 } },
        { "empty", new int[0], new int[0] },
        { "single", new int[] { 5 }, new int[] { 5 } },
        { "two", new int[] { 1, 2 }, new int[] { 2, 1 } },
        { "duplicates", new int[] { 1, 1, 2 }, new int[] { 2, 1, 1 } },
        { "palindrome", new int[] { 1, 2, 1 }, new int[] { 1, 2, 1 } },
        { "negative", new int[] { -1, 0, -3 }, new int[] { -3, 0, -1 } },
        { "limit", new int[] { 2147483647, int.MinValue, 0 }, new int[] { 0, int.MinValue, 2147483647 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int[] want)
    {
        Assert.NotNull(name);
        var items0 = (int[])items.Clone();
        var got = ReverseList.Of(items);
        Assert.Equal(want, got);
        Assert.NotSame(items, got);
        Assert.Equal(items0, items);
    }
}
