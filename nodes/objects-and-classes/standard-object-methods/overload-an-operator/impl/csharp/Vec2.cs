// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

using System;
using System.Collections.Generic;

/// <summary>A 2D vector. The record struct generates == and !=; the type defines +.</summary>
public readonly record struct Vec2(long X, long Y)
{
    public static readonly Vec2 Zero = new(0, 0);

    public static Vec2 operator +(Vec2 a, Vec2 b) => new(a.X + b.X, a.Y + b.Y);

    public static long[] VectorSum(IReadOnlyList<long> xs, IReadOnlyList<long> ys)
    {
        if (xs.Count != ys.Count)
        {
            throw new ArgumentException("the lists must have the same length");
        }
        var total = Zero;
        for (var i = 0; i < xs.Count; i++)
        {
            total += new Vec2(xs[i], ys[i]);
        }
        return new[] { total.X, total.Y };
    }
}
