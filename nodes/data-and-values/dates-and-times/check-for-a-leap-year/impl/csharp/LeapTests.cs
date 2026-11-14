// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

using System;
using Xunit;

public sealed class LeapTests
{
    public static TheoryData<string, int, bool> Cases => new()
    {
        { "divisible-by-4", 2024, true },
        { "not-divisible-by-4", 2023, false },
        { "century", 1900, false },
        { "divisible-by-400", 2000, true },
        { "next-century", 2100, false },
        { "year-1600", 1600, true },
        { "year-1700", 1700, false },
        { "year-2400", 2400, true },
        { "year-1", 1, false },
        { "year-4", 4, true },
        { "year-100", 100, false },
        { "year-1500-julian-leap", 1500, false },
        { "year-1582", 1582, false },
        { "year-9999", 9999, false },
        { "year-zero", 0, true },
        { "year-minus-1", -1, false },
        { "year-minus-4", -4, true },
        { "year-minus-100", -100, false },
        { "year-minus-400", -400, true },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void IsLeapYear(string name, int year, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Leap.IsLeapYear(year));
    }

    [Fact]
    public void SameAsDateTimeInItsRange()
    {
        for (var year = 1; year <= 9999; year++)
        {
            Assert.Equal(DateTime.IsLeapYear(year), Leap.IsLeapYear(year));
        }
    }

    [Fact]
    public void DateTimeRejectsYearZero()
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => DateTime.IsLeapYear(0));
    }
}
