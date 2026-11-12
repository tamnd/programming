// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

using System.Collections.Generic;
using System.Text;

/// <summary>Removes each code point of a set from a string.</summary>
public static class CharRemover
{
    /// <summary>
    /// Returns s without the code points of chars. EnumerateRunes gives code
    /// points, so a surrogate pair is one Rune.
    /// </summary>
    public static string RemoveChars(string s, string chars)
    {
        var remove = new HashSet<Rune>(chars.EnumerateRunes());
        var sb = new StringBuilder(s.Length);
        foreach (Rune r in s.EnumerateRunes())
        {
            if (!remove.Contains(r))
            {
                sb.Append(r.ToString());
            }
        }
        return sb.ToString();
    }
}
