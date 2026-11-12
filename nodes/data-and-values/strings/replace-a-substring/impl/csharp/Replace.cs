// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

using System;

/// <summary>Replaces a substring in a string. The search is ordinal.</summary>
public static class Replace
{
    /// <summary>Returns s with each match of old replaced by nw.</summary>
    public static string All(string s, string old, string nw)
    {
        return s.Replace(old, nw, StringComparison.Ordinal);
    }

    /// <summary>Returns s with the first match of old replaced by nw.</summary>
    public static string First(string s, string old, string nw)
    {
        int i = s.IndexOf(old, StringComparison.Ordinal);
        if (i < 0)
        {
            return s;
        }
        return string.Concat(s.AsSpan(0, i), nw, s.AsSpan(i + old.Length));
    }
}
