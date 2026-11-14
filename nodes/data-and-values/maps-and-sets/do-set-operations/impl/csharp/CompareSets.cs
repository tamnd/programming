// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

using System;
using System.Collections.Generic;
using System.Linq;

public static class CompareSets
{
    // Run returns the union, intersection, difference and symmetric difference
    // of the sets of a and b, each sorted, and whether the set of a is a subset
    // of the set of b.
    public static (string[] Union, string[] Intersection, string[] Difference, string[] SymmetricDifference, bool IsSubset)
        Run(IEnumerable<string> a, IEnumerable<string> b)
    {
        var setA = new HashSet<string>(a, StringComparer.Ordinal);
        var setB = new HashSet<string>(b, StringComparer.Ordinal);
        // UnionWith, IntersectWith, ExceptWith and SymmetricExceptWith change
        // the set, so each one works on a copy.
        var union = new HashSet<string>(setA, StringComparer.Ordinal);
        union.UnionWith(setB);
        var intersection = new HashSet<string>(setA, StringComparer.Ordinal);
        intersection.IntersectWith(setB);
        var difference = new HashSet<string>(setA, StringComparer.Ordinal);
        difference.ExceptWith(setB);
        var symmetric = new HashSet<string>(setA, StringComparer.Ordinal);
        symmetric.SymmetricExceptWith(setB);
        return (Sorted(union), Sorted(intersection), Sorted(difference), Sorted(symmetric), setA.IsSubsetOf(setB));
    }

    private static string[] Sorted(HashSet<string> set) => set.Order(StringComparer.Ordinal).ToArray();
}
