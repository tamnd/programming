// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

using System;
using System.Linq;

public static class SortByLength
{
    // Sort returns the words in order of the length, then of the word.
    public static string[] Sort(string[] words)
    {
        // StringComparer.Ordinal compares the code units, not the rules of a culture.
        return words.OrderBy(w => w.Length).ThenBy(w => w, StringComparer.Ordinal).ToArray();
    }
}
