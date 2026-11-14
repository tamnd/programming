// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

using System;
using System.Collections.Generic;
using System.Linq;

public static class MergeMaps
{
    // Merge returns the entries of both dictionaries in key order; the value of over wins.
    public static (string Key, int Value)[] Merge(Dictionary<string, int> baseMap, Dictionary<string, int> over)
    {
        // The copy keeps baseMap as it is. The indexer replaces the value of a key that is there.
        var merged = new Dictionary<string, int>(baseMap);
        foreach (var (key, value) in over)
        {
            merged[key] = value;
        }
        return merged.OrderBy(p => p.Key, StringComparer.Ordinal).Select(p => (p.Key, p.Value)).ToArray();
    }
}
