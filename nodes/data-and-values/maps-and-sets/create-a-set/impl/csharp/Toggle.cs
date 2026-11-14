// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

using System;
using System.Collections.Generic;
using System.Linq;

public static class Toggle
{
    // Run adds each word that is not in the set, and removes each word that is in it.
    public static string[] Run(IEnumerable<string> words)
    {
        var seen = new HashSet<string>(StringComparer.Ordinal);
        foreach (var word in words)
        {
            // Remove returns true if the set had the word.
            if (!seen.Remove(word))
            {
                seen.Add(word);
            }
        }
        return seen.Order(StringComparer.Ordinal).ToArray();
    }
}
