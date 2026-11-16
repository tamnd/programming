// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

/// <summary>Named constants for a count of seconds.</summary>
public static class Seconds
{
    // A const field gets its value at compile time, and the compiler puts
    // the value in each place that uses the name.
    public const long SecondsPerMinute = 60;
    public const long MinutesPerHour = 60;
    public const long HoursPerDay = 24;
    public const long SecondsPerDay = SecondsPerMinute * MinutesPerHour * HoursPerDay;

    /// <summary>Returns the number of seconds in days days of 24 hours.</summary>
    public static long SecondsIn(long days) => days * SecondsPerDay;
}
