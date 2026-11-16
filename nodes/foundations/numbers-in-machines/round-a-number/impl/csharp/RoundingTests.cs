// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.
using System;
using Xunit;

public sealed class RoundingTests
{
    public static TheoryData<string, double, long> Cases => new()
    {
        { "down", 2.4, 2 },
        { "up", 2.6, 3 },
        { "negative", -2.6, -3 },
        { "integer", 7.0, 7 },
        { "half-even-up", 2.5, 3 },
        { "half-odd-up", 1.5, 2 },
        { "half-zero", 0.5, 1 },
        { "half-negative", -2.5, -3 },
        { "half-negative-small", -0.5, -1 },
        { "negative-below-half", -0.4, 0 },
        { "near-half", 0.49999999999999994, 0 },
        { "zero", 0.0, 0 },
        { "large-odd", 4503599627370497.0, 4503599627370497 },
        { "large-half-negative", -1000000000000000.5, -1000000000000001 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, double x, long want)
    {
        Assert.True(want == Rounding.RoundHalfAway(x), name);
    }

    [Fact]
    public void DefaultIsToEven()
    {
        Assert.Equal(2.0, Math.Round(2.5));
        Assert.Equal(-2.0, Math.Round(-2.5));
        Assert.Equal(3.0, Math.Round(2.5, MidpointRounding.AwayFromZero));
    }
}
