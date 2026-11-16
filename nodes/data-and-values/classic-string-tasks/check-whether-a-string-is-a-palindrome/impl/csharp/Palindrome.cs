// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

/// <summary>Checks whether a text is a palindrome, with case, spaces and punctuation ignored.</summary>
public static class Palindrome
{
    /// <summary>
    /// Returns true if the ASCII letters and digits of text, in lower case,
    /// read the same backwards. Two indexes move toward the middle, so the
    /// method makes no copy of the text.
    /// </summary>
    public static bool IsPalindrome(string text)
    {
        var i = 0;
        var j = text.Length - 1;
        while (i < j)
        {
            if (!char.IsAsciiLetterOrDigit(text[i]))
            {
                i++;
            }
            else if (!char.IsAsciiLetterOrDigit(text[j]))
            {
                j--;
            }
            else if (char.ToLowerInvariant(text[i]) != char.ToLowerInvariant(text[j]))
            {
                return false;
            }
            else
            {
                i++;
                j--;
            }
        }
        return true;
    }
}
