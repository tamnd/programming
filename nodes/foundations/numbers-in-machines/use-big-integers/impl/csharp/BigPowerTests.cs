// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.
using System;
using System.Numerics;
using Xunit;

public sealed class BigPowerTests
{
    public static TheoryData<string, int, int, string> Cases => new()
    {
        { "small", 2, 10, "1024" },
        { "two-to-the-100", 2, 100, "1267650600228229401496703205376" },
        { "ten-to-the-30", 10, 30, "1" + new string('0', 30) },
        { "zero-exponent", 7, 0, "1" },
        { "zero-to-the-zero", 0, 0, "1" },
        { "zero-base", 0, 5, "0" },
        { "minus-one-odd", -1, 199, "-1" },
        { "negative-odd", -3, 41, "-36472996377170786403" },
        { "negative-even", -3, 40, "12157665459056928801" },
        { "int64-max-plus-one", 2, 63, "9223372036854775808" },
        { "uint64-max-plus-one", 2, 64, "18446744073709551616" },
        { "two-to-the-200", 2, 200, "1606938044258990275541962092341162602522202993782792835301376" },
        { "largest", 1000, 200, "1" + new string('0', 600) },
        { "smallest", -1000, 199, "-1" + new string('0', 597) },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int @base, int exp, string want)
    {
        Assert.True(want == BigMath.BigPower(@base, exp), name);
    }

    [Fact]
    public void FixedWidth()
    {
        BigInteger big = BigInteger.Pow(2, 63);
        Assert.Equal(long.MaxValue, (long)(big - 1));
        Assert.Throws<OverflowException>(() => (long)big);
        Assert.Equal(9223372036854775808UL, (ulong)big);
        Assert.Throws<OverflowException>(() => (ulong)(big * 2));
    }
}
