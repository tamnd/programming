// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

using System.Linq;
using Xunit;

public sealed class GroupByLengthTests
{
    public static TheoryData<string, string[], int[], string[][]> Cases => new()
    {
        { "fruits", new string[] { "apple", "fig", "kiwi", "pear", "plum", "date" }, new[] { 3, 4, 5 }, new string[][] { new[] { "fig" }, new[] { "kiwi", "pear", "plum", "date" }, new[] { "apple" } } },
        { "letters", new string[] { "a", "bb", "c" }, new[] { 1, 2 }, new string[][] { new[] { "a", "c" }, new[] { "bb" } } },
        { "empty", new string[0], new int[0], new string[0][] },
        { "single", new string[] { "x" }, new[] { 1 }, new string[][] { new[] { "x" } } },
        { "same-length", new string[] { "cat", "dog", "cow" }, new[] { 3 }, new string[][] { new[] { "cat", "dog", "cow" } } },
        { "order", new string[] { "ccc", "a", "bb" }, new[] { 1, 2, 3 }, new string[][] { new[] { "a" }, new[] { "bb" }, new[] { "ccc" } } },
        { "duplicates", new string[] { "ab", "ab", "c" }, new[] { 1, 2 }, new string[][] { new[] { "c" }, new[] { "ab", "ab" } } },
        { "long", new string[] { "abcdefghij", "abcdefghi", "a" }, new[] { 1, 9, 10 }, new string[][] { new[] { "a" }, new[] { "abcdefghi" }, new[] { "abcdefghij" } } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] words, int[] lengths, string[][] groups)
    {
        Assert.NotNull(name);
        var got = GroupByLength.Group(words);
        Assert.Equal(lengths, got.Select(g => g.Length));
        Assert.Equal(groups, got.Select(g => g.Words));
    }
}
