// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

using System;
using Xunit;

public sealed class DurationFormatTests
{
    public static TheoryData<string, long, string> Cases => new()
    {
        { "zero", 0L, "00:00:00" },
        { "seconds-only", 59L, "00:00:59" },
        { "one-minute", 60L, "00:01:00" },
        { "one-hour", 3600L, "01:00:00" },
        { "hours-minutes-seconds", 3723L, "01:02:03" },
        { "just-under-an-hour", 3599L, "00:59:59" },
        { "just-under-a-day", 86399L, "23:59:59" },
        { "one-day", 86400L, "24:00:00" },
        { "more-than-a-day", 90061L, "25:01:01" },
        { "hundred-hours", 360000L, "100:00:00" },
        { "one-week", 604800L, "168:00:00" },
        { "one-year", 31536000L, "8760:00:00" },
        { "minus-one-second", -1L, "-00:00:01" },
        { "negative-minute", -61L, "-00:01:01" },
        { "negative-hours", -3723L, "-01:02:03" },
        { "negative-day", -90061L, "-25:01:01" },
        { "beyond-go-duration", 10000000000L, "2777777:46:40" },
        { "largest", 1000000000000000L, "277777777777:46:40" },
        { "most-negative", -1000000000000000L, "-277777777777:46:40" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Format(string name, long seconds, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, DurationFormat.FormatDuration(seconds));
    }

    [Fact]
    public void TimeSpanRange()
    {
        Assert.Equal("1.01:01:01", TimeSpan.FromSeconds(90061).ToString());
        Assert.Throws<OverflowException>(() => TimeSpan.FromSeconds(1e15));
    }
}
