// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

using Xunit;

public sealed class TakeTests
{
    public static TheoryData<string, int[], int, int[]> Cases => new()
    {
        { "first-three", new[] { 1, 2, 3, 4, 5 }, 3, new[] { 1, 2, 3 } },
        { "first-two", new[] { 10, 20, 30, 40 }, 2, new[] { 10, 20 } },
        { "zero", new[] { 1, 2 }, 0, new int[0] },
        { "empty", new int[0], 3, new int[0] },
        { "more-than-length", new[] { 1, 2 }, 5, new[] { 1, 2 } },
        { "equal-length", new[] { 4, 5, 6 }, 3, new[] { 4, 5, 6 } },
        { "one", new[] { 9, 8 }, 1, new[] { 9 } },
        { "negative", new[] { -1, -2, -3 }, 2, new[] { -1, -2 } },
        { "extremes", new[] { -2147483648, 2147483647, 0 }, 2, new[] { -2147483648, 2147483647 } },
    };

    public static TheoryData<string, int, int, int[]> Infinite => new()
    {
        { "from-zero", 0, 5, new[] { 0, 1, 2, 3, 4 } },
        { "from-ten", 10, 3, new[] { 10, 11, 12 } },
        { "none", 7, 0, new int[0] },
        { "one", 0, 1, new[] { 0 } },
        { "large-start", 999998, 2, new[] { 999998, 999999 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void First(string name, int[] items, int n, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Take.First(items, n));
    }

    [Theory]
    [MemberData(nameof(Infinite))]
    public void FirstNaturals(string name, int start, int n, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Take.FirstNaturals(start, n));
    }

    [Fact]
    public void FirstReturnsACopy()
    {
        int[] items = { 1, 2, 3 };
        Take.First(items, 2)[0] = 99;
        Assert.Equal(new[] { 1, 2, 3 }, items);
    }
}
