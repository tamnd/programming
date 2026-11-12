// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

using System.Text;

/// <summary>Repeats a string.</summary>
public static class StringRepeat
{
    /// <summary>
    /// Returns n copies of s. The string type has no repeat method for a
    /// string, so the code appends s to a StringBuilder with the full capacity.
    /// </summary>
    public static string Repeat(string s, int n)
    {
        var sb = new StringBuilder(s.Length * n);
        for (int i = 0; i < n; i++)
        {
            sb.Append(s);
        }
        return sb.ToString();
    }
}
