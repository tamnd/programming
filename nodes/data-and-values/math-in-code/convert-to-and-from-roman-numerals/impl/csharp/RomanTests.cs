// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

using System;
using Xunit;

public sealed class RomanTests
{
    public static TheoryData<string, long, string> Cases => new()
    {
        { "year-1990", 1990, "MCMXC" },
        { "year-2008", 2008, "MMVIII" },
        { "each-symbol-once", 1666, "MDCLXVI" },
        { "one", 1, "I" },
        { "four", 4, "IV" },
        { "nine", 9, "IX" },
        { "forty-nine", 49, "XLIX" },
        { "ninety-nine", 99, "XCIX" },
        { "four-hundred-forty-four", 444, "CDXLIV" },
        { "year-1994", 1994, "MCMXCIV" },
        { "longest", 3888, "MMMDCCCLXXXVIII" },
        { "largest", 3999, "MMMCMXCIX" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void ToRoman(string name, long n, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Roman.ToRoman(n));
        Assert.Equal(n, Roman.FromRoman(want));
    }

    [Fact]
    public void RoundTrip()
    {
        for (int n = 1; n <= 3999; n++)
        {
            Assert.Equal(n, Roman.FromRoman(Roman.ToRoman(n)));
        }
    }

    [Theory]
    [InlineData("")]
    [InlineData("IIII")]
    [InlineData("IC")]
    [InlineData("VX")]
    [InlineData("MMMM")]
    [InlineData("iv")]
    [InlineData("ABC")]
    [InlineData("IIV")]
    [InlineData("XM")]
    public void Invalid(string s)
    {
        Assert.Throws<FormatException>(() => Roman.FromRoman(s));
    }

    [Theory]
    [InlineData(0)]
    [InlineData(-1)]
    [InlineData(4000)]
    public void OutOfRange(long n)
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => Roman.ToRoman(n));
    }
}
