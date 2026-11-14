// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

using System.Globalization;

/// <summary>Reads the current date and time and writes it in UTC.</summary>
public static class Now
{
    /// <summary>Returns the UTC date and time of epochNs as "YYYY-MM-DDTHH:MM:SS.mmmZ".</summary>
    public static string UtcTimestamp(long epochNs)
    {
        // The / of C# rounds toward zero. Step back one millisecond for a negative reading with a rest.
        long ms = epochNs / 1_000_000;
        if (epochNs % 1_000_000 < 0)
        {
            ms--;
        }
        // The invariant culture keeps the digits and the separators fixed. fff truncates; it does not round.
        return DateTimeOffset.FromUnixTimeMilliseconds(ms)
            .ToString("yyyy'-'MM'-'dd'T'HH':'mm':'ss'.'fff'Z'", CultureInfo.InvariantCulture);
    }

    /// <summary>Reads the clock of time once and returns the UTC date and time of the reading.</summary>
    /// <remarks>TimeProvider.System reads DateTimeOffset.UtcNow. A test can give a fake TimeProvider.</remarks>
    public static string NowUtc(TimeProvider? time = null)
    {
        time ??= TimeProvider.System;
        DateTimeOffset now = time.GetUtcNow();
        // A tick is 100 nanoseconds.
        long ns = checked((now.UtcTicks - DateTimeOffset.UnixEpoch.UtcTicks) * 100);
        return UtcTimestamp(ns);
    }
}
