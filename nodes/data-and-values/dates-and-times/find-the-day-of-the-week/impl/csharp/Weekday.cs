// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

using System;
using System.Globalization;

/// <summary>Finds the day of the week and the ISO week of a date.</summary>
public static class Weekday
{
    /// <summary>Returns the ISO 8601 day of the week: 1 for Monday to 7 for Sunday.</summary>
    public static int DayOfWeekIso(int year, int month, int day)
    {
        // The DayOfWeek enumeration has Sunday = 0, so 0 becomes 7.
        var weekday = (int)new DateOnly(year, month, day).DayOfWeek;
        return weekday == 0 ? 7 : weekday;
    }

    /// <summary>Returns the ISO 8601 week-based year and week number of a date.</summary>
    public static (int WeekYear, int Week) IsoWeek(int year, int month, int day)
    {
        var date = new DateTime(year, month, day);
        return (ISOWeek.GetYear(date), ISOWeek.GetWeekOfYear(date));
    }
}
