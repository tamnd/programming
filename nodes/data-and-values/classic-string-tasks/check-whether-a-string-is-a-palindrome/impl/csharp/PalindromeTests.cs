// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

using Xunit;

public sealed class PalindromeTests
{
    public static TheoryData<string, string, bool> Cases => new()
    {
        { "racecar", "racecar", true },
        { "panama", "A man, a plan, a canal: Panama", true },
        { "hello", "hello", false },
        { "empty", "", true },
        { "only-punctuation", "?! ...", true },
        { "one-letter", "a", true },
        { "two-letters", "ab", false },
        { "mixed-case", "Aa", true },
        { "nixon", "No 'x' in Nixon", true },
        { "cat-i-saw", "Was it a car or a cat I saw?", true },
        { "digits", "12321", true },
        { "digits-not", "123", false },
        { "zero-p", "0P", false },
        { "race-a-car", "race a car", false },
        { "even-near", "abca", false },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void IsPalindrome(string name, string text, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Palindrome.IsPalindrome(text));
    }
}
