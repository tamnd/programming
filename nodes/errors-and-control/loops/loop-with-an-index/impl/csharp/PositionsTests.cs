// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

using Xunit;

public sealed class PositionsTests
{
    public static TheoryData<string, string[], string, int[]> Cases => new()
    {
        { "two-matches", new[] { "a", "b", "a", "c" }, "a", new[] { 0, 2 } },
        { "words", new[] { "red", "green", "blue", "green" }, "green", new[] { 1, 3 } },
        { "empty", new string[0], "a", new int[0] },
        { "none", new[] { "a", "b" }, "z", new int[0] },
        { "all", new[] { "x", "x", "x" }, "x", new[] { 0, 1, 2 } },
        { "last", new[] { "a", "b", "c" }, "c", new[] { 2 } },
        { "case", new[] { "A", "a" }, "a", new[] { 1 } },
        { "empty-string", new[] { "", "a", "" }, "", new[] { 0, 2 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] items, string target, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Positions.Of(items, target));
    }
}
