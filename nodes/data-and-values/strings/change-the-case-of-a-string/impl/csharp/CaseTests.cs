// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

using Xunit;

public sealed class CaseTests
{
    // Only the ASCII letters change. U+0131 is the dotless i, U+0130 is the capital I with a dot, U+212A is the KELVIN SIGN, and U+FF41 is the fullwidth a.
    public static TheoryData<string, string, string> Upper => new()
    {
        { "word", "hello", "HELLO" },
        { "mixed", "Hello, World!", "HELLO, WORLD!" },
        { "already", "ABC", "ABC" },
        { "empty", "", "" },
        { "digits-punct", "a1-b2_c3", "A1-B2_C3" },
        { "non-ascii", "caf\u00e9", "CAF\u00e9" },
        { "sharp-s", "stra\u00dfe", "STRA\u00dfE" },
        { "dotless-i", "\u0131 i", "\u0131 I" },
        { "greek", "\u03c3", "\u03c3" },
        { "fullwidth", "\uff41", "\uff41" },
        { "astral", "a\U0001f642b", "A\U0001f642B" },
        { "combining", "cafe\u0301", "CAFE\u0301" },
    };

    public static TheoryData<string, string, string> Lower => new()
    {
        { "lower-word", "HELLO", "hello" },
        { "lower-mixed", "Hello, World!", "hello, world!" },
        { "lower-empty", "", "" },
        { "lower-dotted-i", "\u0130", "\u0130" },
        { "lower-sigma", "\u03a3\u0391\u03a3", "\u03a3\u0391\u03a3" },
        { "lower-kelvin", "\u212a", "\u212a" },
        { "lower-astral", "A\U0001f642B", "a\U0001f642b" },
    };

    [Theory]
    [MemberData(nameof(Upper))]
    public void ToUpper(string name, string s, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, AsciiCase.ToUpper(s));
    }

    [Theory]
    [MemberData(nameof(Lower))]
    public void ToLower(string name, string s, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, AsciiCase.ToLower(s));
    }
}
