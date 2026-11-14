// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

using System.Collections.Generic;
using System.Linq;

public static class Lookup
{
    // Get returns the value of each key in table, or fallback for a key not in table.
    public static int[] Get(IReadOnlyDictionary<string, int> table, string[] keys, int fallback)
    {
        // GetValueOrDefault does not add the key. The indexer would throw
        // KeyNotFoundException for a missing key.
        return keys.Select(key => table.GetValueOrDefault(key, fallback)).ToArray();
    }
}
