// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-unix-timestamp/.

using Xunit;

public sealed class UnixTimeTests
{
    public static TheoryData<string, int, int, int, int, int, int, int, long> Cases => new()
    {
        { "epoch", 1970, 1, 1, 0, 0, 0, 0, 0L },
        { "billennium", 2001, 9, 9, 1, 46, 40, 0, 1000000000L },
        { "negative-offset", 2024, 3, 10, 2, 0, 0, -300, 1710054000L },
        { "positive-offset", 2024, 3, 10, 16, 0, 0, 540, 1710054000L },
        { "one-second-before-epoch", 1969, 12, 31, 23, 59, 59, 0, -1L },
        { "offset-moves-before-epoch", 1970, 1, 1, 0, 0, 0, 60, -3600L },
        { "offset-moves-to-next-year", 2023, 12, 31, 19, 0, 0, -300, 1704067200L },
        { "offset-of-45-minutes", 2024, 1, 1, 5, 45, 0, 345, 1704067200L },
        { "offset-of-minus-9-30", 2023, 12, 31, 14, 30, 0, -570, 1704067200L },
        { "offset-plus-14", 1970, 1, 1, 14, 0, 0, 840, 0L },
        { "offset-minus-12", 1969, 12, 31, 12, 0, 0, -720, 0L },
        { "leap-day", 2024, 2, 29, 0, 0, 0, 0, 1709164800L },
        { "year-2000-is-leap", 2000, 2, 29, 0, 0, 0, 0, 951782400L },
        { "year-1900-is-not-leap", 1900, 3, 1, 0, 0, 0, 0, -2203891200L },
        { "int32-max", 2038, 1, 19, 3, 14, 7, 0, 2147483647L },
        { "after-int32-max", 2038, 1, 19, 3, 14, 8, 0, 2147483648L },
        { "int32-min", 1901, 12, 13, 20, 45, 52, 0, -2147483648L },
        { "year-99-is-not-1999", 99, 12, 31, 23, 59, 59, 0, -59011459201L },
        { "first-second-of-year-1", 1, 1, 1, 0, 0, 0, 0, -62135596800L },
        { "last-second-of-year-9999", 9999, 12, 31, 23, 59, 59, 0, 253402300799L },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void UnixTimestamp(string name, int year, int month, int day, int hour, int minute, int second, int offset, long want)
    {
        Assert.Equal(want, UnixTime.UnixTimestamp(year, month, day, hour, minute, second, offset));
        var back = DateTimeOffset.FromUnixTimeSeconds(want).ToOffset(TimeSpan.FromMinutes(offset));
        Assert.Equal(new DateTime(year, month, day, hour, minute, second), back.DateTime);
        Assert.NotNull(name);
    }

    [Fact]
    public void Now()
    {
        long seconds = UnixTime.NowUnixSeconds();
        long millis = UnixTime.NowUnixMillis();
        Assert.True(seconds >= 1_704_067_200L);
        long diff = millis / 1000 - seconds;
        Assert.InRange(diff, 0L, 1L);
    }
}
