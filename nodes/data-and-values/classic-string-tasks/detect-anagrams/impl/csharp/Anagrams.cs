// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

using System;

/// <summary>Checks whether two texts are anagrams.</summary>
public static class Anagrams
{
    /// <summary>
    /// Returns true if a and b hold each letter from a to z the same number
    /// of times. Case, spaces, punctuation and digits do not count.
    /// </summary>
    public static bool AreAnagrams(string a, string b)
    {
        Span<int> counts = stackalloc int[26];
        foreach (var c in a)
        {
            if (char.IsAsciiLetter(c))
            {
                counts[char.ToLowerInvariant(c) - 'a']++;
            }
        }
        foreach (var c in b)
        {
            if (char.IsAsciiLetter(c))
            {
                counts[char.ToLowerInvariant(c) - 'a']--;
            }
        }
        foreach (var n in counts)
        {
            if (n != 0)
            {
                return false;
            }
        }
        return true;
    }
}
