// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

using Xunit;

public sealed class ManhattanTests
{
    public static TheoryData<string, int[], int[], int> Cases => new()
    {
        { "first", new[] { 1, 2 }, new[] { 4, 6 }, 7 },
        { "on-a-line", new[] { 0, 0 }, new[] { 10, 0 }, 10 },
        { "down-left", new[] { 5, 5 }, new[] { 2, 1 }, 7 },
        { "mixed-signs", new[] { 3, -1 }, new[] { -2, 4 }, 10 },
        { "same-point", new[] { 7, 7 }, new[] { 7, 7 }, 0 },
        { "negative", new[] { -3, -4 }, new[] { -1, -1 }, 5 },
        { "large", new[] { 1000000, -1000000 }, new[] { -1000000, 1000000 }, 4000000 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] p, int[] q, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Manhattan.Distance(p, q));
    }
}
