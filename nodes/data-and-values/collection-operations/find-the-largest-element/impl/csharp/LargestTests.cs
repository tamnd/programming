// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

using System;
using Xunit;

public sealed class LargestTests
{
    public static TheoryData<string, int[], int> Cases => new()
    {
        { "digits", new int[] { 3, 1, 4, 1, 5 }, 5 },
        { "middle", new int[] { 10, 20, 15 }, 20 },
        { "single", new int[] { 7 }, 7 },
        { "all-negative", new int[] { -3, -1, -2 }, -1 },
        { "first", new int[] { 9, 2, 3 }, 9 },
        { "last", new int[] { 1, 2, 9 }, 9 },
        { "duplicates", new int[] { 5, 9, 9, 1 }, 9 },
        { "all-same", new int[] { 4, 4, 4 }, 4 },
        { "zero", new int[] { -5, 0, -1 }, 0 },
        { "extremes", new int[] { -2147483648, 2147483647, 0 }, 2147483647 },
        { "min-only", new int[] { -2147483648 }, -2147483648 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Largest.Find(items));
    }

    [Fact]
    public void EmptyThrows()
    {
        Assert.Throws<InvalidOperationException>(() => Largest.Find(new int[0]));
    }
}
