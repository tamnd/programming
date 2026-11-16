// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

using System;
using Xunit;

public sealed class ComplexArithTests
{
    public static TheoryData<string, double[], double[], double[]> Cases => new()
    {
        { "textbook", new double[] { 3.0, 4.0 }, new double[] { 1.0, -2.0 }, new double[] { 4.0, 2.0, 11.0, -2.0, -1.0, 2.0, 5.0, 0.9272952180016122 } },
        { "rosetta", new double[] { 1.0, 1.0 }, new double[] { 3.14159, 1.25 }, new double[] { 4.14159, 2.25, 1.8915899999999999, 4.39159, 0.38414593243590134, 0.16546321590504276, 1.4142135623730951, 0.7853981633974483 } },
        { "real-only", new double[] { 6.0, 0.0 }, new double[] { 3.0, 0.0 }, new double[] { 9.0, 0.0, 18.0, 0.0, 2.0, 0.0, 6.0, 0.0 } },
        { "imaginary-unit", new double[] { 0.0, 1.0 }, new double[] { 0.0, 1.0 }, new double[] { 0.0, 2.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.5707963267948966 } },
        { "negative-real", new double[] { -2.0, 0.0 }, new double[] { 1.0, 0.0 }, new double[] { -1.0, 0.0, -2.0, 0.0, -2.0, 0.0, 2.0, 3.141592653589793 } },
        { "zero", new double[] { 0.0, 0.0 }, new double[] { 2.0, -3.0 }, new double[] { 2.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } },
        { "negative-phase", new double[] { 1.0, -1.0 }, new double[] { -1.0, -1.0 }, new double[] { 0.0, -2.0, -2.0, 0.0, 0.0, 1.0, 1.4142135623730951, -0.7853981633974483 } },
        { "imaginary-divisor", new double[] { 2.0, 4.0 }, new double[] { 0.0, 2.0 }, new double[] { 2.0, 6.0, -8.0, 4.0, 2.0, -1.0, 4.47213595499958, 1.1071487177940904 } },
        { "large-divisor", new double[] { 1e+150, 1e+150 }, new double[] { 1e+155, 1e+155 }, new double[] { 1.00001e+155, 1.00001e+155, 0.0, 2e+305, 9.999999999999999e-06, 0.0, 1.414213562373095e+150, 0.7853981633974483 } },
    };

    private static bool Close(double got, double want) =>
        Math.Abs(got - want) <= Math.Max(1e-9, 1e-12 * Math.Abs(want));

    [Theory]
    [MemberData(nameof(Cases))]
    public void ComplexArithmetic(string name, double[] a, double[] b, double[] want)
    {
        Assert.NotNull(name);
        double[] got = ComplexArith.ComplexArithmetic(a, b);
        Assert.Equal(8, got.Length);
        for (int i = 0; i < 8; i++)
        {
            Assert.True(Close(got[i], want[i]), $"item {i} = {got[i]}");
        }
    }
}
