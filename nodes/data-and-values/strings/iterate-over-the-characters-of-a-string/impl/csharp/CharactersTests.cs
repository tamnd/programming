// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

using Xunit;

public sealed class CharactersTests
{
    // A character is a code point. U+1F642 is one character, but two UTF-16 code units and four UTF-8 bytes. A combining mark, a ZWJ and each regional indicator is a character of its own.
    public static TheoryData<string, string, string[]> Cases => new()
    {
        { "ascii", "abc", new string[] { "a", "b", "c" } },
        { "word", "hello", new string[] { "h", "e", "l", "l", "o" } },
        { "space", "a b", new string[] { "a", " ", "b" } },
        { "empty", "", new string[] {  } },
        { "single", "x", new string[] { "x" } },
        { "non-ascii", "h\u00e9", new string[] { "h", "\u00e9" } },
        { "cjk", "\u65e5\u672c", new string[] { "\u65e5", "\u672c" } },
        { "astral", "a\U0001f642b", new string[] { "a", "\U0001f642", "b" } },
        { "combining", "e\u0301", new string[] { "e", "\u0301" } },
        { "regional-indicator", "\U0001f1fa\U0001f1f8", new string[] { "\U0001f1fa", "\U0001f1f8" } },
        { "zwj", "\U0001f469\u200d\U0001f4bb", new string[] { "\U0001f469", "\u200d", "\U0001f4bb" } },
        { "control", "a\tb\nc", new string[] { "a", "\t", "b", "\n", "c" } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Characters(string name, string s, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, StringCharacters.Characters(s));
    }
}
