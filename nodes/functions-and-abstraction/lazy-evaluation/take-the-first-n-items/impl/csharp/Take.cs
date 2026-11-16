// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

using System.Collections.Generic;
using System.Linq;

public static class Take
{
    // First returns a copy of the first n items; a shorter array gives all its items.
    public static int[] First(int[] items, int n)
    {
        // Take stops at the end of the sequence, so n can be larger than the length.
        return items.Take(n).ToArray();
    }

    // Naturals is the infinite sequence start, start + 1, ... It runs only when it is read.
    private static IEnumerable<int> Naturals(int start)
    {
        for (int i = start; ; i++)
        {
            yield return i;
        }
    }

    // FirstNaturals returns the first n items of the infinite sequence.
    public static int[] FirstNaturals(int start, int n)
    {
        return Naturals(start).Take(n).ToArray();
    }
}
