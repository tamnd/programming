// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

using System.Linq;

public static class ZipPairs
{
    // Pairs returns the pairs (a[i], b[i]) up to the end of the shorter array.
    public static (int First, int Second)[] Pairs(int[] a, int[] b)
    {
        // Zip stops at the end of the shorter sequence.
        return a.Zip(b).ToArray();
    }
}
