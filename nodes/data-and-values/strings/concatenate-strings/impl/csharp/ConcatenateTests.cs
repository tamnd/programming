// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

using Xunit;

public sealed class ConcatenateTests
{
    public static TheoryData<string, string, string, string> Cases => new()
    {
        { "hello-world", "Hello, ", "world", "Hello, world" },
        { "two-words", "good", "bye", "goodbye" },
        { "keep-spaces", "a ", " b", "a  b" },
        { "empty-both", "", "", "" },
        { "empty-left", "", "abc", "abc" },
        { "empty-right", "abc", "", "abc" },
        { "non-ascii", "naï", "ve", "naïve" },
        { "astral", "a", "🙂", "a🙂" },
        { "combining-mark", "e", "\u0301", "e\u0301" },
        { "flag-halves", "🇫", "🇷", "🇫🇷" },
        { "digit-strings", "1", "2", "12" },
    };

    public static TheoryData<string, string, int, string> NumberCases => new()
    {
        { "positive", "item", 3, "item3" },
        { "zero", "n=", 0, "n=0" },
        { "negative", "x", -5, "x-5" },
        { "empty-prefix", "", 42, "42" },
        { "int32-max", "max=", 2147483647, "max=2147483647" },
        { "int32-min", "min=", -2147483648, "min=-2147483648" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Concat(string name, string a, string b, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Concatenate.Concat(a, b));
    }

    [Theory]
    [MemberData(nameof(NumberCases))]
    public void ConcatNumber(string name, string s, int n, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Concatenate.ConcatNumber(s, n));
    }
}
