// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

using System;
using System.Collections.Generic;

public static class RoundRobin
{
    // Returns the job numbers in the order in which round-robin finishes them.
    public static List<int> FinishOrder(IReadOnlyList<int> work, int slice)
    {
        var left = new int[work.Count];
        var queue = new Queue<int>();
        for (int i = 0; i < work.Count; i++)
        {
            left[i] = work[i];
            queue.Enqueue(i);
        }
        var done = new List<int>();
        while (queue.TryDequeue(out int job))
        {
            left[job] -= Math.Min(slice, left[job]);
            if (left[job] == 0)
            {
                done.Add(job);
            }
            else
            {
                queue.Enqueue(job);
            }
        }
        return done;
    }
}
