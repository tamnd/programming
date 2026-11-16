// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

using Xunit;

public sealed class AnagramsTests
{
    public static TheoryData<string, string, string, bool> Cases => new()
    {
        { "listen-silent", "listen", "silent", true },
        { "dormitory", "Dormitory", "dirty room", true },
        { "hello-world", "hello", "world", false },
        { "both-empty", "", "", true },
        { "one-empty", "a", "", false },
        { "same-set-other-counts", "aab", "abb", false },
        { "extra-letter", "abc", "abcd", false },
        { "same-length-other-letters", "abc", "abd", false },
        { "case", "Listen", "SILENT", true },
        { "same-word", "stop", "stop", true },
        { "gentleman", "a gentleman", "elegant man", true },
        { "they-see", "The eyes!", "They see.", true },
        { "punctuation-only", "!?", "...", true },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void AreAnagrams(string name, string a, string b, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Anagrams.AreAnagrams(a, b));
        Assert.Equal(want, Anagrams.AreAnagrams(b, a));
    }
}
