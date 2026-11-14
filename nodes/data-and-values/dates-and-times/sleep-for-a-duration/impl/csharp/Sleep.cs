// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

/// <summary>Sleeps for a duration on a monotonic clock.</summary>
public static class Sleep
{
    /// <summary>
    /// Sleeps until the clock reaches the deadline. A sleep that ends early is followed by a sleep
    /// for the rest of the time.
    /// </summary>
    public static void SleepLoop(long durationNs, Func<long> clock, Action<long> sleep)
    {
        long now = clock();
        long deadline = now + durationNs;
        while (now < deadline)
        {
            sleep(deadline - now);
            now = clock();
        }
    }

    /// <summary>Blocks the thread for at least durationNs nanoseconds.</summary>
    public static void SleepFor(long durationNs) =>
        SleepLoop(durationNs, NowNs, ns => Thread.Sleep(ToMilliseconds(ns)));

    /// <summary>Completes after at least durationNs nanoseconds. The thread is free in the meantime.</summary>
    public static async Task SleepForAsync(long durationNs, CancellationToken cancel = default)
    {
        long now = NowNs();
        long deadline = now + durationNs;
        while (now < deadline)
        {
            await Task.Delay(ToMilliseconds(deadline - now), cancel).ConfigureAwait(false);
            now = NowNs();
        }
    }

    /// <summary>Runs SleepLoop on a fake clock and returns the durations it asks for.</summary>
    public static long[] SleepRequests(long durationNs, long startNs, long[] readings)
    {
        var times = new List<long> { startNs };
        times.AddRange(readings);
        int next = 0;
        var requests = new List<long>();
        SleepLoop(durationNs, () => times[next++], requests.Add);
        return requests.ToArray();
    }

    /// <summary>
    /// Rounds ns up to whole milliseconds, because Thread.Sleep and Task.Delay take milliseconds,
    /// and cuts the result to int.MaxValue. A value of -1 would mean Timeout.Infinite.
    /// </summary>
    private static int ToMilliseconds(long ns)
    {
        long ms = ns / 1_000_000 + (ns % 1_000_000 > 0 ? 1 : 0);
        return (int)Math.Min(ms, int.MaxValue);
    }

    /// <summary>Reads the monotonic clock of Stopwatch in nanoseconds.</summary>
    private static long NowNs() =>
        (long)((Int128)Stopwatch.GetTimestamp() * 1_000_000_000 / Stopwatch.Frequency);
}
