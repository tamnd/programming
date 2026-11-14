// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

using System;
using System.Globalization;

/// <summary>Formats a duration in seconds as HH:MM:SS.</summary>
public static class DurationFormat
{
    /// <summary>
    /// Returns the duration as HH:MM:SS. The hours do not wrap at 24. The method divides a long, so
    /// it has no TimeSpan and no TimeSpan limit.
    /// </summary>
    public static string FormatDuration(long seconds)
    {
        string sign = seconds < 0 ? "-" : "";
        long a = Math.Abs(seconds);
        return string.Format(
            CultureInfo.InvariantCulture, "{0}{1:00}:{2:00}:{3:00}", sign, a / 3600, a % 3600 / 60, a % 60);
    }
}
