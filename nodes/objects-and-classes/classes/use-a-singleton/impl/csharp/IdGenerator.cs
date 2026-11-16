// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

using System;
using System.Collections.Generic;
using System.Threading;

/// <summary>Hands out consecutive ids. Get the one instance with IdGenerator.Instance.</summary>
public sealed class IdGenerator
{
    // Lazy<T> runs the factory once, on the first read of Value, and is
    // thread-safe by default.
    private static readonly Lazy<IdGenerator> lazy = new(() => new IdGenerator());
    private static int constructions;
    private long next;

    private IdGenerator() // private: no other class can make one
    {
        Interlocked.Increment(ref constructions);
    }

    public static IdGenerator Instance => lazy.Value;

    public static int Constructions => Volatile.Read(ref constructions);

    public long NextId() => Interlocked.Increment(ref next);

    public static long[] Report(int calls)
    {
        var seen = new List<IdGenerator>();
        long first = 0, last = 0;
        for (var i = 0; i < calls; i++)
        {
            var g = Instance;
            if (!seen.Exists(s => ReferenceEquals(s, g)))
            {
                seen.Add(g);
            }
            last = g.NextId();
            if (i == 0)
            {
                first = last;
            }
        }
        return [last - first, Constructions, seen.Count];
    }
}
