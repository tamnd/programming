// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

using Xunit;

public sealed class ZipJoinTests
{
    public static TheoryData<string, string[], string[], int[], string[]> Cases => new()
    {
        { "rosetta", new[] { "a", "b", "c" }, new[] { "A", "B", "C" }, new[] { 1, 2, 3 }, new[] { "aA1", "bB2", "cC3" } },
        { "words", new[] { "x", "y" }, new[] { "X", "Y" }, new[] { 10, 20 }, new[] { "xX10", "yY20" } },
        { "shorter-first", new[] { "a" }, new[] { "A", "B" }, new[] { 1, 2, 3 }, new[] { "aA1" } },
        { "shorter-last", new[] { "a", "b", "c" }, new[] { "A", "B", "C" }, new[] { 1 }, new[] { "aA1" } },
        { "one-empty", new[] { "a", "b" }, new string[0], new[] { 1, 2 }, new string[0] },
        { "all-empty", new string[0], new string[0], new int[0], new string[0] },
        { "negative", new[] { "a" }, new[] { "A" }, new[] { -5 }, new[] { "aA-5" } },
        { "empty-strings", new[] { "", "b" }, new[] { "A", "" }, new[] { 1, 2 }, new[] { "A1", "b2" } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] lower, string[] upper, int[] digits, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, ZipJoin.Of(lower, upper, digits));
    }
}
