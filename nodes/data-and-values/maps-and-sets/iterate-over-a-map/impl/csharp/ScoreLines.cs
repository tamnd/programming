// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

using System;
using System.Collections.Generic;
using System.Linq;

public static class ScoreLines
{
    // Lines returns a line key=value for each entry in key order, then the total of the values.
    public static List<string> Lines(Dictionary<string, int> scores)
    {
        var lines = new List<string>();
        // The order of a Dictionary is undefined. StringComparer.Ordinal compares the chars.
        foreach (var (key, value) in scores.OrderBy(p => p.Key, StringComparer.Ordinal))
        {
            lines.Add($"{key}={value}");
        }
        var total = 0;
        foreach (var value in scores.Values)
        {
            total += value;
        }
        lines.Add($"total={total}");
        return lines;
    }
}
