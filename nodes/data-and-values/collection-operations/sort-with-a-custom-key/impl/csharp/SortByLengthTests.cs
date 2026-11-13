// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

using Xunit;

public sealed class SortByLengthTests
{
    public static TheoryData<string, string[], string[]> Cases => new()
    {
        { "fruits", new string[] { "pear", "fig", "apple", "kiwi" }, new string[] { "fig", "kiwi", "pear", "apple" } },
        { "three", new string[] { "bb", "a", "ccc" }, new string[] { "a", "bb", "ccc" } },
        { "empty", new string[0], new string[0] },
        { "single", new string[] { "x" }, new string[] { "x" } },
        { "same-length", new string[] { "dog", "cat", "ant" }, new string[] { "ant", "cat", "dog" } },
        { "duplicates", new string[] { "b", "a", "b" }, new string[] { "a", "b", "b" } },
        { "sorted", new string[] { "a", "bb", "ccc" }, new string[] { "a", "bb", "ccc" } },
        { "reverse", new string[] { "ccc", "bb", "a" }, new string[] { "a", "bb", "ccc" } },
        { "long", new string[] { "abcdefghij", "abcdefghi", "z" }, new string[] { "z", "abcdefghi", "abcdefghij" } },
        { "prefix", new string[] { "ab", "a", "abc", "b" }, new string[] { "a", "b", "ab", "abc" } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] words, string[] want)
    {
        Assert.NotNull(name);
        var given = (string[])words.Clone();
        Assert.Equal(want, SortByLength.Sort(words));
        Assert.Equal(given, words);
    }
}
