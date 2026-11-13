// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

using Xunit;

public sealed class GraphemeCountTests
{
    // A flag is 2 regional indicators, and U+1F469 U+200D U+1F4BB is 3 code points. Each of them is one grapheme.
    public static TheoryData<string, string, int> Cases => new()
    {
        { "empty", "", 0 },
        { "ascii", "abc", 3 },
        { "precomposed", "\u00e9", 1 },
        { "emoji", "\U0001f642", 1 },
        { "combining", "e\u0301", 1 },
        { "two-marks", "e\u0301\u0302", 1 },
        { "leading-mark", "\u0301a", 2 },
        { "crlf", "\r\n", 1 },
        { "lfcr", "\n\r", 2 },
        { "cr-mark", "\r\u0301", 2 },
        { "soft-hyphen", "a\u00adb", 3 },
        { "flag", "\U0001f1eb\U0001f1f7", 1 },
        { "three-indicators", "\U0001f1eb\U0001f1f7\U0001f1ea", 2 },
        { "two-flags", "\U0001f1eb\U0001f1f7\U0001f1ea\U0001f1f8", 2 },
        { "zwj-sequence", "\U0001f469\u200d\U0001f4bb", 1 },
        { "zwj-after-letter", "a\u200d\U0001f642", 2 },
        { "skin-tone", "\U0001f44d\U0001f3fd", 1 },
        { "variation-selector", "\u2764\ufe0f", 1 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void CountGraphemes(string name, string s, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Graphemes.Count(s));
    }
}
