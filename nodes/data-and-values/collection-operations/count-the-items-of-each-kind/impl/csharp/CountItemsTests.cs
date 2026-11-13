// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

using Xunit;

public sealed class CountItemsTests
{
    public static TheoryData<string, int[], (int, int)[]> Cases => new()
    {
        { "mixed", new int[] { 3, 1, 3, 2, 1, 3 }, new (int, int)[] { (1, 2), (2, 1), (3, 3) } },
        { "same", new int[] { 5, 5, 5 }, new (int, int)[] { (5, 3) } },
        { "empty", new int[0], new (int, int)[0] },
        { "single", new int[] { 7 }, new (int, int)[] { (7, 1) } },
        { "negative", new int[] { -1, 2, -1, 0 }, new (int, int)[] { (-1, 2), (0, 1), (2, 1) } },
        { "distinct", new int[] { 4, 2, 9 }, new (int, int)[] { (2, 1), (4, 1), (9, 1) } },
        { "order", new int[] { 9, 8, 9 }, new (int, int)[] { (8, 1), (9, 2) } },
        { "zero", new int[] { 0, 0 }, new (int, int)[] { (0, 2) } },
        { "extremes", new int[] { 2147483647, -2147483648, 2147483647 }, new (int, int)[] { (-2147483648, 1), (2147483647, 2) } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, (int, int)[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, CountItems.Count(items));
    }
}
