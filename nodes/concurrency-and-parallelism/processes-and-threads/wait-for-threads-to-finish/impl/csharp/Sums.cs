// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

using System.Linq;
using System.Threading.Tasks;

/// <summary>Sums each chunk on a thread-pool thread, waits for all of them and collects the sums.</summary>
public static class Sums
{
    public static long[] ParallelSums(long[][] chunks)
    {
        // Task.Run queues each sum to the thread pool and returns a Task<long> at once.
        Task<long>[] tasks = chunks.Select(chunk => Task.Run(() => chunk.Sum())).ToArray();
        // WhenAll completes when every task has completed; its result keeps the order of the tasks.
        return Task.WhenAll(tasks).GetAwaiter().GetResult();
    }
}
