// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

public static class UnixTime
{
    /// <summary>Returns the seconds since 1970-01-01T00:00:00Z of a local time with the given UTC offset.</summary>
    public static long UnixTimestamp(int year, int month, int day, int hour, int minute, int second, int offsetMinutes)
    {
        var offset = TimeSpan.FromMinutes(offsetMinutes);
        return new DateTimeOffset(year, month, day, hour, minute, second, offset).ToUnixTimeSeconds();
    }

    /// <summary>Returns the current Unix time in whole seconds.</summary>
    public static long NowUnixSeconds() => DateTimeOffset.UtcNow.ToUnixTimeSeconds();

    /// <summary>Returns the current Unix time in whole milliseconds.</summary>
    public static long NowUnixMillis() => DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
}
