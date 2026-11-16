// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

using System;
using System.Numerics;
using Xunit;

public sealed class GcdTests
{
    public static TheoryData<string, long, long, long, long> Cases => new()
    {
        { "common-factor", 12, 18, 6, 36 },
        { "coprime", 17, 5, 1, 85 },
        { "euclid-example", 48, 180, 12, 720 },
        { "zero-and-number", 0, 7, 7, 0 },
        { "both-zero", 0, 0, 0, 0 },
        { "negative", -12, 18, 6, 36 },
        { "both-negative", -4, -6, 2, 12 },
        { "equal", 9, 9, 9, 9 },
        { "one", 1, 1000000007, 1, 1000000007 },
        { "multiple", 5, 25, 5, 25 },
        { "fibonacci", 832040, 514229, 1, 427859097160 },
        { "big-lcm", 3000000000000000000, 6000000000000000000, 3000000000000000000, 6000000000000000000 },
        { "largest", 9223372036854775807, -9223372036854775807, 9223372036854775807, 9223372036854775807 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void GcdAndLcm(string name, long a, long b, long gcd, long lcm)
    {
        Assert.NotNull(name);
        Assert.Equal((gcd, lcm), Gcd.GcdAndLcm(a, b));
        Assert.Equal((gcd, lcm), Gcd.GcdAndLcm(b, a));
        Assert.Equal(new BigInteger(gcd), BigInteger.GreatestCommonDivisor(a, b));
    }

    [Fact]
    public void SmallestValueThrows()
    {
        Assert.Throws<OverflowException>(() => Gcd.GcdAndLcm(long.MinValue, 0));
    }
}
