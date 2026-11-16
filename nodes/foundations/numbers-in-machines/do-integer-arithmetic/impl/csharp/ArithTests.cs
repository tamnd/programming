// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.
using System;
using Xunit;

public sealed class ArithTests
{
    public static TheoryData<string, long, long, int, long[]> Cases => new()
    {
        { "small", 7, 2, 3, new long[] { 9, 5, 14, 3, 1, 343 } },
        { "exact-division", 20, 5, 2, new long[] { 25, 15, 100, 4, 0, 400 } },
        { "negative-dividend", -7, 2, 3, new long[] { -5, -9, -14, -3, -1, -343 } },
        { "negative-divisor", 7, -2, 2, new long[] { 5, 9, -14, -3, 1, 49 } },
        { "both-negative", -7, -2, 1, new long[] { -9, -5, 14, 3, -1, -7 } },
        { "zero-dividend", 0, 5, 0, new long[] { 5, -5, 0, 0, 0, 1 } },
        { "zero-power", 5, 3, 0, new long[] { 8, 2, 15, 1, 2, 1 } },
        { "divisor-larger", 3, 7, 1, new long[] { 10, -4, 21, 0, 3, 3 } },
        { "largest", 100000, 100000, 3, new long[] { 200000, 0, 10000000000, 1, 0, 1000000000000000 } },
        { "smallest", -100000, 1, 3, new long[] { -99999, -100001, -100000, -100000, 0, -1000000000000000 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, long a, long b, int n, long[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Arithmetic.Arith(a, b, n));
    }

    [Fact]
    public void CaretIsNotAPower()
    {
        Assert.Equal(1, 2 ^ 3);
        Assert.Equal(8.0, Math.Pow(2, 3));
    }
}
