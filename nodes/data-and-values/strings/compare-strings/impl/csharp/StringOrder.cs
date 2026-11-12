// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

using System;

/// <summary>Compares strings in the order of their code points.</summary>
public static class StringOrder
{
    // string.CompareOrdinal compares UTF-16 code units, so the loop compares
    // code points.
    public static int Compare(string a, string b)
    {
        int i = 0;
        while (i < a.Length && i < b.Length)
        {
            int x = char.ConvertToUtf32(a, i);
            int y = char.ConvertToUtf32(b, i);
            if (x != y)
            {
                return x < y ? -1 : 1;
            }
            i += x > 0xFFFF ? 2 : 1;
        }
        return Math.Sign(a.Length - b.Length);
    }

    public static bool AreEqual(string a, string b) => string.Equals(a, b, StringComparison.Ordinal);
}
