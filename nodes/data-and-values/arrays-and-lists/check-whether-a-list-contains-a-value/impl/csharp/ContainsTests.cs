// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

using Xunit;

public sealed class ContainsTests
{
    public static TheoryData<string, int[], int, bool> Cases => new()
    {
        { "present", new int[] { 1, 2, 3 }, 2, true },
        { "absent", new int[] { 1, 2, 3 }, 4, false },
        { "empty", new int[0], 0, false },
        { "single-yes", new int[] { 5 }, 5, true },
        { "single-no", new int[] { 5 }, 0, false },
        { "first", new int[] { 7, 8, 9 }, 7, true },
        { "last", new int[] { 7, 8, 9 }, 9, true },
        { "duplicates", new int[] { 2, 2, 2 }, 2, true },
        { "negative", new int[] { -1, -2, -3 }, -2, true },
        { "zero-absent", new int[] { -1, 1 }, 0, false },
        { "limit", new int[] { 2147483647, int.MinValue }, int.MinValue, true },
        { "limit-absent", new int[] { 2147483647 }, int.MinValue, false },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int value, bool want)
    {
        Assert.NotNull(name);
        var items0 = (int[])items.Clone();
        Assert.Equal(want, Contains.Of(items, value));
        Assert.Equal(items0, items);
    }
}
