// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

using Xunit;

public sealed class SortIntsTests
{
    public static TheoryData<string, int[], int[]> Cases => new()
    {
        { "small", new int[] { 3, 1, 2 }, new int[] { 1, 2, 3 } },
        { "five", new int[] { 5, 4, 1, 3, 2 }, new int[] { 1, 2, 3, 4, 5 } },
        { "empty", new int[0], new int[0] },
        { "single", new int[] { 1 }, new int[] { 1 } },
        { "sorted", new int[] { 1, 2, 3 }, new int[] { 1, 2, 3 } },
        { "reversed", new int[] { 9, 8, 7 }, new int[] { 7, 8, 9 } },
        { "duplicates", new int[] { 2, 1, 2, 1 }, new int[] { 1, 1, 2, 2 } },
        { "negative", new int[] { -1, -10, 5 }, new int[] { -10, -1, 5 } },
        { "digits", new int[] { 10, 9, 100, 1 }, new int[] { 1, 9, 10, 100 } },
        { "extremes", new int[] { 2147483647, -2147483648, 0 }, new int[] { -2147483648, 0, 2147483647 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, SortInts.Sort(items));
    }
}
