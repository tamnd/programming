// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

using Xunit;

public sealed class AppendToListTests
{
    public static TheoryData<string, int[], int, int, int[]> Cases => new()
    {
        { "at-end", new int[] { 1, 2 }, 2, 3, new int[] { 1, 2, 3 } },
        { "at-front", new int[] { 2, 3 }, 0, 1, new int[] { 1, 2, 3 } },
        { "in-middle", new int[] { 1, 3 }, 1, 2, new int[] { 1, 2, 3 } },
        { "empty", new int[0], 0, 7, new int[] { 7 } },
        { "single-front", new int[] { 5 }, 0, 4, new int[] { 4, 5 } },
        { "single-end", new int[] { 5 }, 1, 6, new int[] { 5, 6 } },
        { "end-of-many", new int[] { 1, 2, 3, 4 }, 4, 5, new int[] { 1, 2, 3, 4, 5 } },
        { "front-of-many", new int[] { 1, 2, 3, 4 }, 0, 0, new int[] { 0, 1, 2, 3, 4 } },
        { "duplicates", new int[] { 1, 1 }, 1, 1, new int[] { 1, 1, 1 } },
        { "limit", new int[] { 2147483647 }, 1, int.MinValue, new int[] { 2147483647, int.MinValue } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int index, int value, int[] want)
    {
        Assert.NotNull(name);
        var items0 = (int[])items.Clone();
        Assert.Equal(want, AppendToList.InsertAt(items, index, value));
        Assert.Equal(items0, items);
    }

    [Fact]
    public void IndexPastTheEnd()
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => AppendToList.InsertAt([1, 2], 3, 9));
    }
}
