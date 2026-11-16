// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

using System;
using Xunit;

public sealed class MeanMedianTests
{
    public static TheoryData<string, long[], double, double> Cases => new()
    {
        { "odd", new long[] { 3, 1, 2 }, 2.0, 2.0 },
        { "even", new long[] { 4, 1, 3, 2 }, 2.5, 2.5 },
        { "skewed", new long[] { 1, 2, 3, 4, 100 }, 22.0, 3.0 },
        { "single", new long[] { 5 }, 5.0, 5.0 },
        { "two-items", new long[] { 1, 2 }, 1.5, 1.5 },
        { "negative", new long[] { -3, -1, -2 }, -2.0, -2.0 },
        { "unsorted-even", new long[] { 10, 2, 8, 4 }, 6.0, 6.0 },
        { "duplicates", new long[] { 1, 1, 1, 5 }, 2.0, 1.0 },
        { "not-an-integer", new long[] { 1, 2, 2 }, 1.6666666666666667, 2.0 },
        { "large", new long[] { 9223372036854775807, 9223372036854775807 }, 9223372036854775808.0, 9223372036854775808.0 },
    };

    private static bool Close(double got, double want) =>
        Math.Abs(got - want) <= Math.Max(1e-9, 1e-12 * Math.Abs(want));

    [Theory]
    [MemberData(nameof(Cases))]
    public void MeanAndMedian(string name, long[] items, double mean, double median)
    {
        Assert.NotNull(name);
        long[] before = (long[])items.Clone();
        var (m, med) = MeanMedian.MeanAndMedian(items);
        Assert.True(Close(m, mean), $"mean = {m}");
        Assert.True(Close(med, median), $"median = {med}");
        Assert.Equal(before, items);
    }

    [Fact]
    public void EmptyThrows()
    {
        Assert.Throws<InvalidOperationException>(() => MeanMedian.MeanAndMedian([]));
    }
}
