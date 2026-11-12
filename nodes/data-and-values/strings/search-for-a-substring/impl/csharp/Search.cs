// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

using System;

/// <summary>Searches a string for a substring, with indexes in code points.</summary>
public static class Search
{
    /// <summary>Returns the code point index of the first match of sub in s, or -1.</summary>
    public static int Find(string s, string sub)
    {
        // IndexOf(string) with no StringComparison uses the current culture,
        // so the code asks for an ordinal search.
        int i = s.IndexOf(sub, StringComparison.Ordinal);
        return i < 0 ? -1 : CodePoints(s, i);
    }

    /// <summary>Returns the code point index of the last match of sub in s, or -1.</summary>
    public static int FindLast(string s, string sub)
    {
        if (sub.Length == 0)
        {
            return CodePoints(s, s.Length);
        }
        int i = s.LastIndexOf(sub, StringComparison.Ordinal);
        return i < 0 ? -1 : CodePoints(s, i);
    }

    // CodePoints counts the code points in the first end UTF-16 code units of s.
    private static int CodePoints(string s, int end)
    {
        int n = 0;
        for (int i = 0; i < end; i++)
        {
            if (!char.IsLowSurrogate(s[i]))
            {
                n++;
            }
        }
        return n;
    }
}
