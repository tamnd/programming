// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

using Xunit;

public sealed class TotalTests
{
    public static TheoryData<string, int[], int> Cases => new()
    {
        { "small", new int[] { 1, 2, 3, 4 }, 10 },
        { "mixed", new int[] { 10, -3, 5 }, 12 },
        { "empty", new int[0], 0 },
        { "single", new int[] { 7 }, 7 },
        { "negative", new int[] { -1, -2, -3 }, -6 },
        { "zero-sum", new int[] { 5, -5 }, 0 },
        { "max", new int[] { 2147483647 }, 2147483647 },
        { "min", new int[] { -2147483648 }, -2147483648 },
        { "to-the-limit", new int[] { 2147483646, 1 }, 2147483647 },
        { "partial-overflow", new int[] { 2147483647, 1, -1 }, 2147483647 },
        { "partial-underflow", new int[] { -2147483648, -1, 1 }, -2147483648 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Total.Of(items));
    }
}
