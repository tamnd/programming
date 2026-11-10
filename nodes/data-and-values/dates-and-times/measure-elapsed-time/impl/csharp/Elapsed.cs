// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

using System.Globalization;

/// <summary>Measures the time that a piece of work takes.</summary>
public static class Elapsed
{
    /// <summary>Runs work once and returns the time it took on the clock of time.</summary>
    /// <remarks>TimeProvider.System uses the time stamp of Stopwatch. A test can give a fake TimeProvider.</remarks>
    public static TimeSpan Measure(Action work, TimeProvider? time = null)
    {
        time ??= TimeProvider.System;
        long start = time.GetTimestamp();
        work();
        return time.GetElapsedTime(start, time.GetTimestamp());
    }

    /// <summary>Returns endNs - startNs as "M.UUU ms", with the rest under one microsecond dropped.</summary>
    public static string ElapsedMs(long startNs, long endNs)
    {
        long ns = endNs - startNs;
        return string.Create(CultureInfo.InvariantCulture, $"{ns / 1_000_000}.{ns / 1_000 % 1_000:D3} ms");
    }
}
