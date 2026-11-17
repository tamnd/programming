// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

using Xunit;

public sealed class CountGreaterTests
{
    public static TheoryData<string, int[], int, int> Cases => new()
    {
        { "some", new[] { 34, 50, 25, 100, 65 }, 50, 2 },
        { "all", new[] { 3, 4, 5 }, 2, 3 },
        { "none", new[] { 3, 4, 5 }, 9, 0 },
        { "empty", new int[0], 0, 0 },
        { "equal-not-counted", new[] { 5, 5, 5 }, 5, 0 },
        { "duplicates", new[] { 7, 1, 7, 7 }, 6, 3 },
        { "negative", new[] { -3, -1, 0, 2 }, -2, 3 },
        { "min-limit", new[] { int.MinValue, 0, int.MaxValue }, int.MinValue, 2 },
        { "max-limit", new[] { int.MaxValue, 1 }, int.MaxValue, 0 },
        { "single", new[] { 1 }, 0, 1 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void CountGreater(string name, int[] items, int limit, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Generic.CountGreater(items, limit));
    }

    [Fact]
    public void Strings()
    {
        Assert.Equal(2, Generic.CountGreater(new[] { "pear", "apple", "plum" }, "orange"));
    }
}
