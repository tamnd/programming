// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

/// <summary>Swap two values with a tuple assignment.</summary>
public static class Swap
{
    /// <summary>
    /// Returns a copy of xs with the items at i and j swapped. The tuple on
    /// the right holds both values before the assignment to the left side.
    /// </summary>
    public static long[] SwapItems(long[] xs, int i, int j)
    {
        var copy = (long[])xs.Clone();
        (copy[i], copy[j]) = (copy[j], copy[i]);
        return copy;
    }
}
