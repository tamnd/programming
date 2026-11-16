// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

using System;

/// <summary>The Levenshtein distance of two strings.</summary>
public static class Levenshtein
{
    /// <summary>
    /// Returns the minimum number of one-character insertions, deletions and
    /// substitutions that change a into b. Two rows of the table are kept.
    /// </summary>
    public static int Distance(string a, string b)
    {
        var prev = new int[b.Length + 1];
        var cur = new int[b.Length + 1];
        for (var j = 0; j <= b.Length; j++)
        {
            prev[j] = j;
        }
        for (var i = 1; i <= a.Length; i++)
        {
            cur[0] = i;
            for (var j = 1; j <= b.Length; j++)
            {
                var cost = a[i - 1] == b[j - 1] ? 0 : 1;
                cur[j] = Math.Min(prev[j - 1] + cost, Math.Min(prev[j], cur[j - 1]) + 1);
            }
            (prev, cur) = (cur, prev);
        }
        return prev[b.Length];
    }
}
