// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

using System;
using Xunit;

public sealed class DateDiffTests
{
    public static TheoryData<string, int, int, int, int, int, int, int> Cases => new()
    {
        { "same-day", 2024, 3, 10, 2024, 3, 10, 0 },
        { "next-day", 2024, 3, 10, 2024, 3, 11, 1 },
        { "common-year", 2023, 1, 1, 2024, 1, 1, 365 },
        { "leap-year", 2024, 1, 1, 2025, 1, 1, 366 },
        { "end-before-start", 2024, 3, 11, 2024, 3, 10, -1 },
        { "across-a-dst-change", 2024, 3, 9, 2024, 3, 11, 2 },
        { "across-leap-day", 2024, 2, 28, 2024, 3, 1, 2 },
        { "month-end-to-next-month", 2024, 1, 31, 2024, 3, 1, 30 },
        { "year-1900-is-not-leap", 1900, 1, 1, 1901, 1, 1, 365 },
        { "year-2000-is-leap", 2000, 1, 1, 2001, 1, 1, 366 },
        { "400-years", 2000, 1, 1, 2400, 1, 1, 146097 },
        { "across-the-epoch", 1969, 12, 31, 1970, 1, 1, 1 },
        { "gregorian-reform-gap", 1582, 10, 4, 1582, 10, 15, 11 },
        { "more-than-292-years", 1700, 1, 1, 2300, 1, 1, 219145 },
        { "whole-range", 1, 1, 1, 9999, 12, 31, 3652058 },
        { "whole-range-backward", 9999, 12, 31, 1, 1, 1, -3652058 },
        { "year-99", 99, 12, 31, 100, 1, 1, 1 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void DaysBetween(string name, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, DateDiff.DaysBetween(startYear, startMonth, startDay, endYear, endMonth, endDay));
    }

    [Fact]
    public void TimeSpanDaysDropsThePartialDay()
    {
        var span = new DateTime(2024, 3, 2, 6, 0, 0) - new DateTime(2024, 3, 1, 18, 0, 0);
        Assert.Equal(0, span.Days);
        Assert.Equal(0.5, span.TotalDays);
    }
}
