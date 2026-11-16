// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

using System;
using Xunit;

public sealed class WrapTests
{
    public static TheoryData<string, string, int, string[]> Cases => new()
    {
        { "fox-ten", "The quick brown fox jumps over the lazy dog", 10, new string[] { "The quick", "brown fox", "jumps over", "the lazy", "dog" } },
        { "fox-twenty", "The quick brown fox jumps over the lazy dog", 20, new string[] { "The quick brown fox", "jumps over the lazy", "dog" } },
        { "sentence", "Wrap text to a width with a greedy loop.", 15, new string[] { "Wrap text to a", "width with a", "greedy loop." } },
        { "empty", "", 10, Array.Empty<string>() },
        { "only-spaces", "   ", 5, Array.Empty<string>() },
        { "fits", "short text", 20, new string[] { "short text" } },
        { "exact-fit", "abcde fghij", 5, new string[] { "abcde", "fghij" } },
        { "long-word", "a supercalifragilistic word", 10, new string[] { "a", "supercalifragilistic", "word" } },
        { "many-spaces", "one   two\tthree\nfour", 9, new string[] { "one two", "three", "four" } },
        { "width-one", "a b c", 1, new string[] { "a", "b", "c" } },
        { "line-exact", "ab cd ef", 5, new string[] { "ab cd", "ef" } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Lines(string name, string text, int width, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Wrap.Lines(text, width));
    }
}
