// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

/// <summary>Counts the letters a to z of a text.</summary>
public static class Letters
{
    /// <summary>
    /// Returns the counts of the letters a to z in text, in the order of the
    /// alphabet. Upper case letters count as lower case letters.
    /// </summary>
    public static long[] CountLetters(string text)
    {
        var counts = new long[26];
        foreach (var ch in text)
        {
            if (char.IsAsciiLetter(ch))
            {
                counts[char.ToLowerInvariant(ch) - 'a']++;
            }
        }
        return counts;
    }
}
