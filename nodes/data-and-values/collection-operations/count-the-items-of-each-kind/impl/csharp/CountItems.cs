// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

using System.Linq;

public static class CountItems
{
    // Count returns one (item, count) pair for each distinct item, in ascending order of the item.
    public static (int Item, int Count)[] Count(int[] items)
    {
        return items
            .GroupBy(x => x)
            .OrderBy(g => g.Key)
            .Select(g => (g.Key, g.Count()))
            .ToArray();
    }
}
