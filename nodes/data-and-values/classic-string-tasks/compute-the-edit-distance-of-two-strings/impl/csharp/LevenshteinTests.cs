// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

using Xunit;

public sealed class LevenshteinTests
{
    public static TheoryData<string, string, string, int> Cases => new()
    {
        { "kitten-sitting", "kitten", "sitting", 3 },
        { "rosettacode", "rosettacode", "raisethysword", 8 },
        { "flaw-lawn", "flaw", "lawn", 2 },
        { "both-empty", "", "", 0 },
        { "empty-left", "", "abc", 3 },
        { "empty-right", "abc", "", 3 },
        { "equal", "same", "same", 0 },
        { "one-substitution", "a", "b", 1 },
        { "transposition", "ab", "ba", 2 },
        { "case", "Apple", "apple", 1 },
        { "disjoint", "abc", "xyz", 3 },
        { "prefix", "abc", "abcdef", 3 },
        { "intention", "intention", "execution", 5 },
        { "space", "a b", "ab", 1 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Distance(string name, string a, string b, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Levenshtein.Distance(a, b));
        Assert.Equal(want, Levenshtein.Distance(b, a));
    }
}
