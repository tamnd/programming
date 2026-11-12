// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

using Xunit;

public sealed class EmptyTests
{
    // U+00A0 is NO-BREAK SPACE, U+200B is ZERO WIDTH SPACE, and U+3000 is IDEOGRAPHIC SPACE. None of them is ASCII white space.
    public static TheoryData<string, string, bool> EmptyCases => new()
    {
        { "letter", "a", false },
        { "word", "hello", false },
        { "empty", "", true },
        { "space", " ", false },
        { "newline", "\n", false },
        { "nbsp", "\u00a0", false },
        { "zero-width", "\u200b", false },
        { "combining", "\u0301", false },
        { "astral", "\U0001f642", false },
        { "cjk", "日", false },
    };

    public static TheoryData<string, string, bool> BlankCases => new()
    {
        { "blank-empty", "", true },
        { "blank-space", " ", true },
        { "blank-mixed", " \t\n\r", true },
        { "blank-vt-ff", "\v\f", true },
        { "blank-letter", "a", false },
        { "blank-padded", "  a  ", false },
        { "blank-nbsp", "\u00a0", false },
        { "blank-zero-width", "\u200b", false },
        { "blank-ideographic", "\u3000", false },
        { "blank-astral", " \U0001f642 ", false },
    };

    [Theory]
    [MemberData(nameof(EmptyCases))]
    public void IsEmpty(string name, string s, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Empty.IsEmpty(s));
    }

    [Theory]
    [MemberData(nameof(BlankCases))]
    public void IsBlank(string name, string s, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Empty.IsBlank(s));
    }
}
