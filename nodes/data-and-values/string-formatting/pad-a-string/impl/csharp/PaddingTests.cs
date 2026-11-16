// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

using Xunit;

public sealed class PaddingTests
{
    public static TheoryData<string, string, int, string, string, string> Cases => new()
    {
        { "zeros", "7", 3, "0", "007", "700" },
        { "spaces", "abc", 6, " ", "   abc", "abc   " },
        { "stars", "abc", 6, "*", "***abc", "abc***" },
        { "empty-string", "", 3, "-", "---", "---" },
        { "width-zero", "abc", 0, " ", "abc", "abc" },
        { "exact-width", "abcd", 4, ".", "abcd", "abcd" },
        { "shorter-width", "hello", 3, " ", "hello", "hello" },
        { "negative-number-text", "-42", 6, "0", "000-42", "-42000" },
        { "fill-same-as-text", "aa", 5, "a", "aaaaa", "aaaaa" },
        { "one-char-wide", "x", 1, "_", "x", "x" },
        { "spaces-inside", "a b", 7, ".", "....a b", "a b...." },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Pad(string name, string s, int width, string fill, string left, string right)
    {
        Assert.NotNull(name);
        Assert.Equal((left, right), Padding.Pad(s, width, fill));
    }
}
