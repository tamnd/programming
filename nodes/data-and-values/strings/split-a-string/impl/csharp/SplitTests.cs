// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

using Xunit;

public sealed class SplitTests
{
    public static TheoryData<string, string, string, int, string[]> Cases => new()
    {
        { "comma", "a,b,c", ",", 0, ["a", "b", "c"] },
        { "csv-line", "Ada,1815,London", ",", 0, ["Ada", "1815", "London"] },
        { "multi-char-separator", "a::b::c", "::", 0, ["a", "b", "c"] },
        { "space-separator", "one two  three", " ", 0, ["one", "two", "", "three"] },
        { "limit-two", "key=value=more", "=", 2, ["key", "value=more"] },
        { "empty", "", ",", 0, [""] },
        { "no-separator", "abc", ",", 0, ["abc"] },
        { "leading-separator", ",a,b", ",", 0, ["", "a", "b"] },
        { "trailing-separators", "a,b,,", ",", 0, ["a", "b", "", ""] },
        { "adjacent-separators", "a,,b", ",", 0, ["a", "", "b"] },
        { "only-separator", ",", ",", 0, ["", ""] },
        { "overlapping-match", "aaa", "aa", 0, ["", "a"] },
        { "dot-separator", "192.168.0.1", ".", 0, ["192", "168", "0", "1"] },
        { "pipe-separator", "a|b|c", "|", 0, ["a", "b", "c"] },
        { "non-ascii", "día→naïve→日本→🙂", "→", 0, ["día", "naïve", "日本", "🙂"] },
        { "limit-one", "a,b,c", ",", 1, ["a,b,c"] },
        { "limit-above-count", "a,b", ",", 5, ["a", "b"] },
        { "limit-keeps-trailing-empty", "a,b,", ",", 3, ["a", "b", ""] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Split(string name, string s, string sep, int limit, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Splitter.Split(s, sep, limit));
    }

    public static TheoryData<string, string, string[]> WhitespaceCases => new()
    {
        { "single-spaces", "a b c", ["a", "b", "c"] },
        { "runs-and-ends", "  lead  and trail  ", ["lead", "and", "trail"] },
        { "tabs-and-newlines", "a\t\tb\nc\n", ["a", "b", "c"] },
        { "one-word", "word", ["word"] },
        { "empty", "", [] },
        { "only-white-space", " \t\n ", [] },
    };

    [Theory]
    [MemberData(nameof(WhitespaceCases))]
    public void SplitWhitespace(string name, string s, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Splitter.SplitWhitespace(s));
    }

    public static TheoryData<string, string, string, string[]> RegexCases => new()
    {
        { "comma-or-semicolon", "a, b;c ,d", "[ ]*[,;][ ]*", ["a", "b", "c", "d"] },
        { "digit-runs", "a1b22c333", "[0-9]+", ["a", "b", "c", ""] },
        { "adjacent-matches", "a,;b", "[,;]", ["a", "", "b"] },
        { "leading-and-trailing", ";a;", ";", ["", "a", ""] },
        { "empty", "", "[,;]", [""] },
        { "no-match", "abc", "[0-9]+", ["abc"] },
    };

    [Theory]
    [MemberData(nameof(RegexCases))]
    public void SplitRegex(string name, string s, string pattern, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Splitter.SplitRegex(s, pattern));
    }
}
