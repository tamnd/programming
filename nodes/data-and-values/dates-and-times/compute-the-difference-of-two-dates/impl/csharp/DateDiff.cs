// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

using System;

/// <summary>Counts the days between two dates.</summary>
public static class DateDiff
{
    /// <summary>Returns the number of days from the start date to the end date.</summary>
    public static int DaysBetween(int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay)
    {
        var start = new DateOnly(startYear, startMonth, startDay);
        var end = new DateOnly(endYear, endMonth, endDay);
        return end.DayNumber - start.DayNumber;
    }
}
