// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

using Xunit;

public sealed class TotalTests
{
    public static TheoryData<string, int[], long> Cases => new()
    {
        { "three", new int[] { 1, 2, 3 }, 6 },
        { "mixed", new int[] { 10, -4, 7, 0, 3 }, 16 },
        { "empty", new int[0], 0 },
        { "single", new int[] { 7 }, 7 },
        { "negative", new int[] { -5, 2, -1 }, -4 },
        { "limit", new int[] { 2147483647, 2147483647, 2 }, 4294967296 },
        { "limit-low", new int[] { int.MinValue, int.MinValue }, -4294967296 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, long want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Total.Of(items));
    }
}
