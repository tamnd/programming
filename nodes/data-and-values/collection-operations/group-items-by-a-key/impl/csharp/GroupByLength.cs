// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

using System.Linq;

public static class GroupByLength
{
    // Group returns one (length, words) pair for each length, in ascending order of the length.
    public static (int Length, string[] Words)[] Group(string[] words)
    {
        return words
            .GroupBy(w => w.Length)
            .OrderBy(g => g.Key)
            .Select(g => (g.Key, g.ToArray()))
            .ToArray();
    }
}
