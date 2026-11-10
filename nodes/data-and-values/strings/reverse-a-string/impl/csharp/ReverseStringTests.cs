// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

using Xunit;

public sealed class ReverseStringTests
{
    public static TheoryData<string, string, string> CodePointCases => new()
    {
        { "ascii", "hello", "olleh" },
        { "two-words", "abc def", "fed cba" },
        { "palindrome", "racecar", "racecar" },
        { "empty", "", "" },
        { "single", "a", "a" },
        { "two-byte", "naïve", "evïan" },
        { "three-byte", "日本語", "語本日" },
        { "astral", "a🙂b", "b🙂a" },
        { "two-astral", "🙂😀", "😀🙂" },
        { "combining-mark", "e\u0301x", "x\u0301e" },
        { "flag", "🇫🇷", "🇷🇫" },
    };

    public static TheoryData<string, string, string> GraphemeCases => new()
    {
        { "ascii", "abc", "cba" },
        { "empty", "", "" },
        { "combining-mark", "e\u0301x", "xe\u0301" },
        { "flags", "🇫🇷🇩🇪", "🇩🇪🇫🇷" },
        { "zwj-family", "a👨\u200d👩\u200d👧b", "b👨\u200d👩\u200d👧a" },
        { "skin-tone", "👍🏽!", "!👍🏽" },
        { "crlf", "a\r\nb", "b\r\na" },
        { "hangul-jamo", "\u1100\u1161x", "x\u1100\u1161" },
    };

    [Theory]
    [MemberData(nameof(CodePointCases))]
    public void ReversesCodePoints(string name, string s, string want)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(want, ReverseString.ReverseCodePoints(s));
    }

    [Theory]
    [MemberData(nameof(GraphemeCases))]
    public void ReversesGraphemes(string name, string s, string want)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(want, ReverseString.ReverseGraphemes(s));
    }
}
