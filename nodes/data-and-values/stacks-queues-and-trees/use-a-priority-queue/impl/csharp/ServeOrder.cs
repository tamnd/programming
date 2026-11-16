// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

using System.Collections.Generic;

public static class ServeOrder
{
    // PriorityQueue is a min-heap, and it does not keep the arrival order of
    // equal priorities. The comparer puts the largest priority first and
    // breaks ties with the request number.
    private static readonly IComparer<(int Priority, int Index)> Order =
        Comparer<(int Priority, int Index)>.Create((x, y) =>
            x.Priority != y.Priority ? y.Priority.CompareTo(x.Priority) : x.Index.CompareTo(y.Index));

    // Returns the request numbers in serve order.
    public static List<int> Of(IReadOnlyList<int> priorities)
    {
        var queue = new PriorityQueue<int, (int Priority, int Index)>(Order);
        for (int i = 0; i < priorities.Count; i++)
        {
            queue.Enqueue(i, (priorities[i], i));
        }
        var served = new List<int>();
        while (queue.TryDequeue(out int request, out _))
        {
            served.Add(request);
        }
        return served;
    }
}
