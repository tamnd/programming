// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-current-date-and-time/.

using System.Globalization;
using System.Text.RegularExpressions;
using Xunit;

public sealed class NowTests
{
    public static TheoryData<string, long, string> Cases => new()
    {
        { "epoch", 0L, "1970-01-01T00:00:00.000Z" },
        { "billennium", 1000000000000000000L, "2001-09-09T01:46:40.000Z" },
        { "with-milliseconds", 1710054000123456789L, "2024-03-10T07:00:00.123Z" },
        { "leap-day", 1709208000000000000L, "2024-02-29T12:00:00.000Z" },
        { "truncate-not-round", 999999L, "1970-01-01T00:00:00.000Z" },
        { "one-ns-before-epoch", -1L, "1969-12-31T23:59:59.999Z" },
        { "one-ms-before-epoch", -1000000L, "1969-12-31T23:59:59.999Z" },
        { "just-over-one-ms-before-epoch", -1000001L, "1969-12-31T23:59:59.998Z" },
        { "last-ms-of-a-year", 1704067199999999999L, "2023-12-31T23:59:59.999Z" },
        { "year-2000-is-leap", 951782400000000000L, "2000-02-29T00:00:00.000Z" },
        { "year-2100-is-not-leap", 4107542399999999999L, "2100-02-28T23:59:59.999Z" },
        { "after-int32-seconds", 2147483648000000000L, "2038-01-19T03:14:08.000Z" },
        { "int64-max", 9223372036854775807L, "2262-04-11T23:47:16.854Z" },
        { "int64-min", long.MinValue, "1677-09-21T00:12:43.145Z" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void UtcTimestamp(string name, long epochNs, string want)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(want, Now.UtcTimestamp(epochNs));
    }

    private sealed class FixedTime(DateTimeOffset now) : TimeProvider
    {
        public int Calls { get; private set; }

        public override DateTimeOffset GetUtcNow()
        {
            Calls++;
            return now;
        }
    }

    [Fact]
    public void NowUtcWithAFakeClock()
    {
        // The offset of the value does not change the instant.
        var time = new FixedTime(new DateTimeOffset(2024, 3, 10, 3, 0, 0, 123, TimeSpan.FromHours(-4)).AddTicks(4567));
        Assert.Equal("2024-03-10T07:00:00.123Z", Now.NowUtc(time));
        Assert.Equal(1, time.Calls);
    }

    [Fact]
    public void NowUtcWithTheRealClock()
    {
        string got = Now.NowUtc();
        Assert.Matches(new Regex(@"^\d{4}-\d\d-\d\dT\d\d:\d\d:\d\d\.\d{3}Z$"), got);
        Assert.True(int.Parse(got[..4], CultureInfo.InvariantCulture) >= 2024, got);
    }
}
