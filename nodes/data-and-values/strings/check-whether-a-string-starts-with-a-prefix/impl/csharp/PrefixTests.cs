// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

using Xunit;

public sealed class PrefixTests
{
    // U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
    public static TheoryData<string, string, string, bool> Starts => new()
    {
        { "yes", "Hello, world", "Hello", true },
        { "no", "Hello, world", "world", false },
        { "whole", "abc", "abc", true },
        { "empty-prefix", "abc", "", true },
        { "empty-s", "", "a", false },
        { "both-empty", "", "", true },
        { "longer-prefix", "ab", "abc", false },
        { "case-sensitive", "Hello", "hello", false },
        { "non-ascii", "caf\u00e9 au lait", "caf\u00e9", true },
        { "cjk", "日本語", "日本", true },
        { "astral", "\U0001f642\U0001f643", "\U0001f642", true },
        { "astral-no", "\U0001f642", "\U0001f643", false },
        { "combining", "cafe\u0301", "cafe", true },
        { "precomposed", "caf\u00e9", "cafe", false },
    };

    public static TheoryData<string, string, string, bool> Ends => new()
    {
        { "ends-yes", "report.txt", ".txt", true },
        { "ends-no", "report.txt", ".csv", false },
        { "ends-empty", "abc", "", true },
        { "ends-longer", "txt", "a.txt", false },
        { "ends-case", "A.TXT", ".txt", false },
        { "ends-astral", "a\U0001f642", "\U0001f642", true },
        { "ends-combining", "cafe\u0301", "e", false },
    };

    [Theory]
    [MemberData(nameof(Starts))]
    public void StartsWith(string name, string s, string prefix, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Prefix.StartsWith(s, prefix));
    }

    [Theory]
    [MemberData(nameof(Ends))]
    public void EndsWith(string name, string s, string suffix, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Prefix.EndsWith(s, suffix));
    }
}
