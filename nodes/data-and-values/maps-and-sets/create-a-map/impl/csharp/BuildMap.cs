// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

using System;
using System.Collections.Generic;
using System.Linq;

public static class BuildMap
{
    // Build puts the pairs in a dictionary, removes the keys in remove, and returns the entries sorted by key.
    public static (string Key, int Value)[] Build((string Key, int Value)[] pairs, string[] remove)
    {
        var map = new Dictionary<string, int>();
        foreach (var (key, value) in pairs)
        {
            // The indexer adds the key, or replaces the value of a key that is there. Add would throw.
            map[key] = value;
        }
        foreach (var key in remove)
        {
            // Remove returns false for a key that is not there.
            map.Remove(key);
        }
        // StringComparer.Ordinal compares the chars. The default comparer depends on the culture.
        return map.OrderBy(p => p.Key, StringComparer.Ordinal).Select(p => (p.Key, p.Value)).ToArray();
    }
}
