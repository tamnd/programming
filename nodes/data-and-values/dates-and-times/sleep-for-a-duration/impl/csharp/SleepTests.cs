// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

using System.Diagnostics;
using System.Threading.Tasks;
using Xunit;

public sealed class SleepTests
{
    public static TheoryData<string, long, long, long[], long[]> Cases => new()
    {
        { "one-call", 1000000L, 0L, new long[] { 1000000L }, new long[] { 1000000L } },
        { "woke-late", 1000000L, 0L, new long[] { 1080000L }, new long[] { 1000000L } },
        { "woke-early", 1000000L, 0L, new long[] { 600000L, 1000000L }, new long[] { 1000000L, 400000L } },
        { "one-second", 1000000000L, 5000000000L, new long[] { 6000050000L }, new long[] { 1000000000L } },
        { "zero", 0L, 0L, new long[] { }, new long[] { } },
        { "negative", -5000000L, 0L, new long[] { }, new long[] { } },
        { "most-negative", -4611686018427387904L, 0L, new long[] { }, new long[] { } },
        { "several-early-wakes", 10000000L, 0L, new long[] { 2000000L, 5000000L, 9999999L, 10000000L }, new long[] { 10000000L, 8000000L, 5000000L, 1L } },
        { "no-progress", 1000000L, 0L, new long[] { 0L, 1000000L }, new long[] { 1000000L, 1000000L } },
        { "exact-deadline", 3000000L, 100L, new long[] { 3000100L }, new long[] { 3000000L } },
        { "one-ns-short", 3000000L, 0L, new long[] { 2999999L, 3000000L }, new long[] { 3000000L, 1L } },
        { "extra-readings", 1000000L, 0L, new long[] { 1000000L, 5000000L }, new long[] { 1000000L } },
        { "negative-origin", 2000000000L, -5000000000L, new long[] { -4000000000L, -3000000000L }, new long[] { 2000000000L, 1000000000L } },
        { "origin-after-100-days", 1500000L, 8640000000000000L, new long[] { 8640000001000000L, 8640000001500000L }, new long[] { 1500000L, 500000L } },
        { "one-nanosecond", 1L, 0L, new long[] { 1000000L }, new long[] { 1L } },
        { "sub-millisecond", 1500000L, 0L, new long[] { 1000000L, 1500000L }, new long[] { 1500000L, 500000L } },
        { "one-hour", 3600000000000L, 0L, new long[] { 3600000000000L }, new long[] { 3600000000000L } },
        { "thirty-days", 2592000000000000L, 0L, new long[] { 2147483647000000L, 2592000000000000L }, new long[] { 2592000000000000L, 444516353000000L } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void SleepRequests(string name, long durationNs, long startNs, long[] readings, long[] want)
    {
        Assert.True(name.Length > 0);
        Assert.Equal(want, Sleep.SleepRequests(durationNs, startNs, readings));
    }

    [Fact]
    public void SleepForWaitsAtLeastTheDuration()
    {
        var watch = Stopwatch.StartNew();
        Sleep.SleepFor(20_000_000);
        Assert.True(watch.Elapsed.Ticks >= 200_000);
    }

    [Theory]
    [InlineData(0L)]
    [InlineData(-5_000_000L)]
    public void SleepForReturnsAtOnce(long durationNs)
    {
        var watch = Stopwatch.StartNew();
        Sleep.SleepFor(durationNs);
        Assert.True(watch.ElapsedMilliseconds < 1000);
    }

    [Fact]
    public async Task TwoSleepsAtTheSameTimeTakeTheTimeOfOne()
    {
        var watch = Stopwatch.StartNew();
        await Task.WhenAll(Sleep.SleepForAsync(100_000_000), Sleep.SleepForAsync(100_000_000));
        Assert.InRange(watch.Elapsed.Ticks, 1_000_000, 1_999_999);
    }
}
