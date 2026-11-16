// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

using System;
using System.Collections.Generic;
using System.Linq;

/// <summary>A generic compose helper.</summary>
public static class Composition
{
    private static readonly Dictionary<string, Func<int, int>> Ops = new()
    {
        ["inc"] = x => x + 1,
        ["dec"] = x => x - 1,
        ["double"] = x => 2 * x,
        ["negate"] = x => -x,
    };

    /// <summary>Returns the composition of the functions. The last one runs first.</summary>
    public static Func<T, T> Compose<T>(params Func<T, T>[] fs)
    {
        return fs.Aggregate((Func<T, T>)(x => x), (f, g) => x => f(g(x)));
    }

    /// <summary>Composes the functions that ops names and applies the result to x.</summary>
    public static int ComposeAll(IReadOnlyList<string> ops, int x)
    {
        return Compose(ops.Select(name => Ops[name]).ToArray())(x);
    }
}
