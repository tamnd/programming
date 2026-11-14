// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

using System;
using System.Globalization;

/// <summary>Converts a local date and time from one time zone to another.</summary>
public static class TzConv
{
    /// <summary>
    /// Returns the instant of a local time in zone. A time in a gap moves forward by the length of the
    /// gap, and a time in an overlap gives the earlier of its two instants. TimeZoneInfo.ConvertTime
    /// throws for a time in a gap, so the offsets are checked here.
    /// </summary>
    public static DateTimeOffset LocalToInstant(DateTime local, TimeZoneInfo zone)
    {
        var t = DateTime.SpecifyKind(local, DateTimeKind.Utc);
        TimeSpan before = zone.GetUtcOffset(t.AddDays(-1));
        TimeSpan after = zone.GetUtcOffset(t.AddDays(1));
        DateTime? instant = null;
        foreach (TimeSpan offset in new[] { before, after })
        {
            DateTime u = t - offset;
            if (zone.GetUtcOffset(u) == offset && (instant is null || u < instant))
            {
                instant = u;
            }
        }
        return new DateTimeOffset(instant ?? t - before, TimeSpan.Zero);
    }

    public static string ConvertTimeZone(
        int year, int month, int day, int hour, int minute, int second, string fromZone, string toZone)
    {
        TimeZoneInfo from = TimeZoneInfo.FindSystemTimeZoneById(fromZone);
        TimeZoneInfo to = TimeZoneInfo.FindSystemTimeZoneById(toZone);
        DateTimeOffset instant = LocalToInstant(new DateTime(year, month, day, hour, minute, second), from);
        return TimeZoneInfo.ConvertTime(instant, to)
            .ToString("yyyy-MM-dd'T'HH:mm:sszzz", CultureInfo.InvariantCulture);
    }
}
