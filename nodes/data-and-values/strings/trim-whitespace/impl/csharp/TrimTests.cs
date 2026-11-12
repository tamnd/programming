// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

using Xunit;

public sealed class TrimTests
{
    // The white space is the 6 ASCII characters: space, tab, line feed, vertical tab, form feed and carriage return. U+00A0, U+3000, U+0085 and U+FEFF stay.
    public static TheoryData<string, string, string> Upper => new()
    {
        { "both-ends", "  hello  ", "hello" },
        { "tab-newline", "\thello world\n", "hello world" },
        { "none", "hello", "hello" },
        { "empty", "", "" },
        { "all-space", " \t\n\r ", "" },
        { "vt-ff", "\v\fx\f\v", "x" },
        { "other-control", "\u0001x\u0001", "\u0001x\u0001" },
        { "no-break-space", "\u00a0x\u00a0", "\u00a0x\u00a0" },
        { "ideographic-space", "\u3000x\u3000", "\u3000x\u3000" },
        { "next-line", "\u0085x\u0085", "\u0085x\u0085" },
        { "bom", "\ufeffx", "\ufeffx" },
        { "astral", " \U0001f642 ", "\U0001f642" },
        { "combining", " \u0301x ", "\u0301x" },
    };

    public static TheoryData<string, string, string> Lower => new()
    {
        { "start-both-ends", "  hello  ", "hello  " },
        { "start-tab-newline", "\n\tx", "x" },
        { "start-empty", "", "" },
        { "start-all-space", "   ", "" },
        { "start-no-break-space", "\u00a0 x", "\u00a0 x" },
        { "start-end-only", "x \t", "x \t" },
    };

    [Theory]
    [MemberData(nameof(Upper))]
    public void Trim(string name, string s, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, AsciiTrim.Trim(s));
    }

    [Theory]
    [MemberData(nameof(Lower))]
    public void TrimStart(string name, string s, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, AsciiTrim.TrimStart(s));
    }
}
