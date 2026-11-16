// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

using Xunit;

public sealed class StringExtensionsTests
{
    public static TheoryData<string, string, int> Cases => new()
    {
        { "three-words", "the quick fox", 3 },
        { "one-word", "hello", 1 },
        { "punctuation", "Hello, world! How are you?", 5 },
        { "empty", "", 0 },
        { "blank", " \t\n ", 0 },
        { "mixed-whitespace", "  two\twords\r\nand three  ", 4 },
        { "non-ascii", "naïve café 東京", 3 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void WordCount(string name, string text, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, text.WordCount());
    }

    [Fact]
    public void ExtensionIsAStaticCall()
    {
        Assert.Equal(2, StringExtensions.WordCount("one two"));
        Assert.Null(typeof(string).GetMethod("WordCount"));
    }
}
