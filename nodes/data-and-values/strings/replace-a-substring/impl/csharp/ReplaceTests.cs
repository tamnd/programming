// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

using Xunit;

public sealed class ReplaceTests
{
    // U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
    public static TheoryData<string, string, string, string, string> All => new()
    {
        { "all", "a-b-c", "-", "+", "a+b+c" },
        { "word", "Hello, world", "world", "there", "Hello, there" },
        { "none", "abc", "x", "y", "abc" },
        { "empty-s", "", "a", "b", "" },
        { "delete", "a-b-c", "-", "", "abc" },
        { "new-contains-old", "aXa", "X", "XXX", "aXXXa" },
        { "overlap", "aaaa", "aa", "b", "bb" },
        { "overlap-odd", "aaa", "aa", "b", "ba" },
        { "case-sensitive", "Hello hello", "hello", "bye", "Hello bye" },
        { "non-ascii", "naïve caf\u00e9", "\u00e9", "e", "naïve cafe" },
        { "astral", "a\U0001f642b\U0001f642", "\U0001f642", "!", "a!b!" },
        { "combining", "cafe\u0301", "e", "E", "cafE\u0301" },
        { "precomposed", "caf\u00e9", "e", "E", "caf\u00e9" },
        { "dot", "a.b.c", ".", "-", "a-b-c" },
        { "dollar", "price", "price", "$&!", "$&!" },
        { "ampersand", "a-b", "-", "&", "a&b" },
        { "backslash", "a/b", "/", "\\", "a\\b" },
    };

    public static TheoryData<string, string, string, string, string> First => new()
    {
        { "first-only", "a-b-c", "-", "+", "a+b-c" },
        { "first-none", "abc", "x", "y", "abc" },
        { "first-overlap", "aaa", "aa", "b", "ba" },
        { "first-astral", "\U0001f642\U0001f642", "\U0001f642", "x", "x\U0001f642" },
        { "first-dollar", "a.b", ".", "$&", "a$&b" },
    };

    [Theory]
    [MemberData(nameof(All))]
    public void All_(string name, string s, string old, string nw, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Replace.All(s, old, nw));
    }

    [Theory]
    [MemberData(nameof(First))]
    public void First_(string name, string s, string old, string nw, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Replace.First(s, old, nw));
    }
}
