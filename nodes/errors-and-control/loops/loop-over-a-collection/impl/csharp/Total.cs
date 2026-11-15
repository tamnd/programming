// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

public static class Total
{
    // Returns the sum of the items, using a foreach loop.
    public static long Of(IEnumerable<int> items)
    {
        // A long sum: an int sum wraps around past int.MaxValue.
        long s = 0;
        foreach (int x in items)
        {
            s += x;
        }
        return s;
    }
}
