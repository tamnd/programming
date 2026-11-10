// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/measure-elapsed-time/.

using Xunit;

public sealed class ElapsedTests
{
    public static TheoryData<string, long, long, string> Cases => new()
    {
        { "one-and-a-half-ms", 1000000000L, 1001500000L, "1.500 ms" },
        { "seconds", 0L, 2345678901L, "2345.678 ms" },
        { "under-one-ms", 50000L, 750000L, "0.700 ms" },
        { "zero", 123456789L, 123456789L, "0.000 ms" },
        { "under-one-microsecond", 0L, 999L, "0.000 ms" },
        { "truncate-not-round", 0L, 1999999L, "1.999 ms" },
        { "leading-zero-in-fraction", 0L, 1005000L, "1.005 ms" },
        { "exact-ms", 0L, 7000000L, "7.000 ms" },
        { "one-hour", 0L, 3600000000000L, "3600000.000 ms" },
        { "negative-readings", -5000000L, -3500000L, "1.500 ms" },
        { "across-zero", -1000000L, 500000L, "1.500 ms" },
        { "beyond-2-pow-53", 9007199254740993L, 9007199255741992L, "1.000 ms" },
        { "near-int64-max", 9223372035854775807L, 9223372036854775807L, "1000.000 ms" },
        { "near-int64-min", long.MinValue, -9223372035854775808L, "1000.000 ms" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void ElapsedMs(string name, long startNs, long endNs, string want)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(want, Elapsed.ElapsedMs(startNs, endNs));
    }

    /// <summary>A clock that counts in nanoseconds and returns fixed readings.</summary>
    private sealed class FakeClock(List<string> calls, params long[] readings) : TimeProvider
    {
        private int next;

        public override long TimestampFrequency => 1_000_000_000;

        public override long GetTimestamp()
        {
            calls.Add("clock");
            return readings[next++];
        }
    }

    [Fact]
    public void MeasureWithAFakeClock()
    {
        var calls = new List<string>();
        var clock = new FakeClock(calls, 5_000_000, 6_500_000);
        Assert.Equal(TimeSpan.FromMilliseconds(1.5), Elapsed.Measure(() => calls.Add("work"), clock));
        Assert.Equal(["clock", "work", "clock"], calls);
    }

    [Fact]
    public void MeasureWithTheMonotonicClock()
    {
        var calls = new List<string>();
        Assert.True(Elapsed.Measure(() => calls.Add("work")) >= TimeSpan.Zero);
        Assert.Equal(["work"], calls);
    }
}
