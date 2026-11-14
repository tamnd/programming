// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

using System.Collections.Generic;
using System.Linq;

// A record struct gets Equals and GetHashCode from its fields, and readonly
// keeps a key from a change while it is in a dictionary.
public readonly record struct Point(int X, int Y);

public static class CountPoints
{
    // Count returns (x, y, count) for each distinct point, in order of x, then y.
    public static (int X, int Y, int Count)[] Count(IEnumerable<Point> points)
    {
        var counts = new Dictionary<Point, int>();
        foreach (var p in points)
        {
            counts[p] = counts.GetValueOrDefault(p) + 1;
        }
        return counts.OrderBy(e => e.Key.X).ThenBy(e => e.Key.Y).Select(e => (e.Key.X, e.Key.Y, e.Value)).ToArray();
    }
}
