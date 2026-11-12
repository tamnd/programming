// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

using Xunit;

public sealed class RemoveTests
{
    // A character is a code point. U+1F642 and U+1F643 have the same high surrogate in UTF-16, and U+00E0 and U+00E9 have the same first byte in UTF-8.
    public static TheoryData<string, string, string, string> Cases => new()
    {
        { "one-char", "banana", "a", "bnn" },
        { "set", "hello, world", "lo", "he, wrd" },
        { "punct", "a-b_c.d", "-_.", "abcd" },
        { "empty", "", "abc", "" },
        { "empty-set", "hello", "", "hello" },
        { "none-found", "hello", "xyz", "hello" },
        { "all", "aaa", "a", "" },
        { "adjacent", "aab", "a", "b" },
        { "astral", "a\U0001f642b\U0001f642", "\U0001f642", "ab" },
        { "astral-other", "\U0001f642\U0001f643", "\U0001f643", "\U0001f642" },
        { "non-ascii", "\u00e0\u00e9", "\u00e9", "\u00e0" },
        { "combining-mark", "e\u0301x", "\u0301", "ex" },
        { "combining-base", "e\u0301", "e", "\u0301" },
        { "composed", "e\u0301", "\u00e9", "e\u0301" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void RemoveChars(string name, string s, string chars, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, CharRemover.RemoveChars(s, chars));
    }
}
