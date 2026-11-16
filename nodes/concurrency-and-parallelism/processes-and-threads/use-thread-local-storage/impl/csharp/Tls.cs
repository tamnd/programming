// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

// Count on a thread-local counter in one thread for each count, with no lock and no shared counter.

using System.Threading;

public static class Tls
{
    // Each thread has its own copy of this static field. Do not give it an initial value here:
    // that would set the copy of one thread only. The copy of every other thread starts at 0.
    [ThreadStatic]
    private static int counter;

    public static int Bump() => ++counter;

    public static int Current() => counter;

    public static int[] CountPerThread(int[] counts)
    {
        var results = new int[counts.Length];
        var threads = new Thread[counts.Length];
        for (int i = 0; i < counts.Length; i++)
        {
            int index = i;
            threads[i] = new Thread(() =>
            {
                for (int k = 0; k < counts[index]; k++)
                {
                    Bump();
                }
                // Each thread writes only its own slot. Join makes the write visible to the caller.
                results[index] = Current();
            });
            threads[i].Start();
        }
        foreach (var t in threads)
        {
            t.Join();
        }
        return results;
    }
}
