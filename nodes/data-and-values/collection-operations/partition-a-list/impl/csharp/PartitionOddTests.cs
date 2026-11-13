// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

using Xunit;

public sealed class PartitionOddTests
{
    public static TheoryData<string, int[], int[], int[]> Cases => new()
    {
        { "mixed", new int[] { 1, 2, 3, 4, 5 }, new int[] { 1, 3, 5 }, new int[] { 2, 4 } },
        { "tens", new int[] { 10, 15, 20, 25 }, new int[] { 15, 25 }, new int[] { 10, 20 } },
        { "empty", new int[0], new int[0], new int[0] },
        { "all-odd", new int[] { 1, 3 }, new int[] { 1, 3 }, new int[0] },
        { "all-even", new int[] { 2, 4 }, new int[0], new int[] { 2, 4 } },
        { "negative", new int[] { -3, -2, -1, 0 }, new int[] { -3, -1 }, new int[] { -2, 0 } },
        { "zero", new int[] { 0 }, new int[0], new int[] { 0 } },
        { "order", new int[] { 9, 8, 1, 2 }, new int[] { 9, 1 }, new int[] { 8, 2 } },
        { "extremes", new int[] { -2147483648, 2147483647 }, new int[] { 2147483647 }, new int[] { -2147483648 } },
        { "duplicates", new int[] { 3, 3, 2, 2 }, new int[] { 3, 3 }, new int[] { 2, 2 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int[] odd, int[] even)
    {
        Assert.NotNull(name);
        var got = PartitionOdd.Split(items);
        Assert.Equal(odd, got.Odd);
        Assert.Equal(even, got.Even);
    }
}
