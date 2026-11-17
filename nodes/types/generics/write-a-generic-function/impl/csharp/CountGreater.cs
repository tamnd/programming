// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

using System;
using System.Collections.Generic;

/// <summary>A generic method with a constraint on its type parameter.</summary>
public static class Generic
{
    /// <summary>Returns how many items are greater than limit.</summary>
    public static int CountGreater<T>(IEnumerable<T> items, T limit)
        where T : IComparable<T>
    {
        int count = 0;
        foreach (T item in items)
        {
            if (item.CompareTo(limit) > 0)
            {
                count++;
            }
        }
        return count;
    }
}
