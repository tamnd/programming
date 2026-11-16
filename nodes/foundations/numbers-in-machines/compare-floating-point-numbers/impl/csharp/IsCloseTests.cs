// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.
using System;
using Xunit;

public sealed class IsCloseTests
{
    public static TheoryData<string, double, double, double, double, bool> Cases => new()
    {
        { "equal", 1.5, 1.5, 1e-9, 0.0, true },
        { "sum-of-tenths", 0.30000000000000004, 0.3, 1e-9, 0.0, true },
        { "different", 1.0, 1.1, 1e-9, 0.0, false },
        { "large-values", 10000000000.0, 10000000001.0, 1e-9, 0.0, true },
        { "zero-and-zero", 0.0, 0.0, 0.0, 0.0, true },
        { "one-ulp-apart", 1.0, 1.0000000000000002, 0.0, 0.0, false },
        { "near-zero-relative", 1e-12, 0.0, 1e-9, 0.0, false },
        { "near-zero-absolute", 1e-12, 0.0, 1e-9, 1e-9, true },
        { "opposite-signs", 1e-10, -1e-10, 1e-9, 0.0, false },
        { "opposite-signs-absolute", 1e-10, -1e-10, 1e-9, 1e-9, true },
        { "on-the-boundary", 10.0, 9.0, 0.1, 0.0, true },
        { "swapped-boundary", 9.0, 10.0, 0.1, 0.0, true },
        { "past-the-boundary", 10.0, 8.875, 0.1, 0.0, false },
        { "negative-values", -1.0, -1.0000000001, 1e-9, 0.0, true },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, double a, double b, double relTol, double absTol, bool want)
    {
        Assert.True(want == FloatCompare.IsClose(a, b, relTol, absTol), name);
        Assert.True(want == FloatCompare.IsClose(b, a, relTol, absTol), name);
    }

    [Fact]
    public void NonFinite()
    {
        Assert.False(FloatCompare.IsClose(double.NaN, double.NaN, 1e-9, 0.0));
        Assert.True(FloatCompare.IsClose(double.PositiveInfinity, double.PositiveInfinity, 1e-9, 0.0));
        Assert.False(FloatCompare.IsClose(double.PositiveInfinity, double.NegativeInfinity, 1e-9, 0.0));
        Assert.False(FloatCompare.IsClose(double.PositiveInfinity, 1e308, 1e-9, 0.0));
    }

    [Fact]
    public void DoubleEpsilonIsNotMachineEpsilon()
    {
        Assert.True(double.Epsilon < 1e-300);
        Assert.False(Math.Abs(0.1 + 0.2 - 0.3) < double.Epsilon);
    }
}
