// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

using System;

/// <summary>Checks for a leap year of the proleptic Gregorian calendar.</summary>
public static class Leap
{
    /// <summary>Returns true for a leap year. DateTime.IsLeapYear accepts only the years 1 to 9999, so the rule is written out.</summary>
    public static bool IsLeapYear(int year)
    {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }
}
