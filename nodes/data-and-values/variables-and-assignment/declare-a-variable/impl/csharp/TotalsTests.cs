// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

using System;
using Xunit;

public sealed class TotalsTests
{
    public static TheoryData<string, long[], long> Cases => new()
    {
        { "one-two-three", new long[] { 1, 2, 3 }, 6 },
        { "four-values", new long[] { 10, 20, 30, 40 }, 100 },
        { "mixed-signs", new long[] { 5, -2, 7 }, 10 },
        { "empty", Array.Empty<long>(), 0 },
        { "single", new long[] { 42 }, 42 },
        { "all-negative", new long[] { -1, -2, -3 }, -6 },
        { "zeros", new long[] { 0, 0, 0 }, 0 },
        { "cancel-out", new long[] { 7, -7 }, 0 },
        { "past-32-bits", new long[] { 4000000000, 5000000000 }, 9000000000 },
        { "eight-values", new long[] { 1, 1, 1, 1, 1, 1, 1, 1 }, 8 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Total(string name, long[] values, long want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Totals.Total(values));
    }
}
