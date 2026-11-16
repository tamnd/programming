// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

using System;
using Xunit;

public sealed class CartesianTests
{
    public static TheoryData<string, double, double, double, double> Cases => new()
    {
        { "east", 1.0, 0.0, 1.0, 0.0 },
        { "north", 2.0, 90.0, 0.0, 2.0 },
        { "west", 2.0, 180.0, -2.0, 0.0 },
        { "diagonal", 1.0, 45.0, 0.7071067811865476, 0.7071067811865476 },
        { "thirty-degrees", 2.0, 30.0, 1.7320508075688772, 1.0 },
        { "zero-radius", 0.0, 123.0, 0.0, 0.0 },
        { "south", 1.0, 270.0, 0.0, -1.0 },
        { "negative-angle", 1.0, -90.0, 0.0, -1.0 },
        { "full-turn", 1.0, 360.0, 1.0, 0.0 },
        { "two-turns", 3.0, 720.0, 3.0, 0.0 },
        { "negative-radius", -1.0, 0.0, -1.0, 0.0 },
        { "large-radius", 1000000.0, 60.0, 500000.0, 866025.4037844386 },
    };

    private static bool Close(double got, double want) =>
        Math.Abs(got - want) <= Math.Max(1e-9, 1e-12 * Math.Abs(want));

    [Theory]
    [MemberData(nameof(Cases))]
    public void ToCartesian(string name, double r, double degrees, double wantX, double wantY)
    {
        Assert.NotNull(name);
        var (x, y) = Cartesian.ToCartesian(r, degrees);
        Assert.True(Close(x, wantX), $"x = {x}");
        Assert.True(Close(y, wantY), $"y = {y}");
    }

    [Fact]
    public void Constants()
    {
        Assert.True(Close(Math.Atan2(0, -1), Math.PI));
        Assert.Equal(4.0, Math.Sqrt(16));
        Assert.Equal(1024.0, Math.Pow(2, 10));
        Assert.True(Close(Math.Log(Math.E), 1));
    }
}
