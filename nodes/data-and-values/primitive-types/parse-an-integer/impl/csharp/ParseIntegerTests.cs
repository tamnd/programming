// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

using System;
using Xunit;

public sealed class ParseIntegerTests
{
    public static TheoryData<string, string, int, long> Cases => new()
    {
        { "decimal", "42", 10, 42 },
        { "negative", "-17", 10, -17 },
        { "hex", "ff", 16, 255 },
        { "binary", "101010", 2, 42 },
        { "base-36", "zz", 36, 1295 },
        { "plus-sign", "+5", 10, 5 },
        { "negative-zero", "-0", 10, 0 },
        { "zero", "0", 10, 0 },
        { "leading-zeros", "007", 10, 7 },
        { "many-leading-zeros", new string('0', 5000) + "42", 10, 42 },
        { "max", "9223372036854775807", 10, 9223372036854775807 },
        { "min", "-9223372036854775808", 10, long.MinValue },
        { "max-hex", "7fffffffffffffff", 16, 9223372036854775807 },
        { "min-binary", "-1000000000000000000000000000000000000000000000000000000000000000", 2, long.MinValue },
        { "upper-case", "FF", 16, 255 },
        { "mixed-case", "Zz", 36, 1295 },
        { "above-safe-integer", "9007199254740993", 10, 9007199254740993 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Parses(string name, string s, int radix, long want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, IntegerParser.ParseInteger(s, radix));
    }

    public static TheoryData<string, string, int> Invalid => new()
    {
        { "empty", "", 10 },
        { "sign-only", "-", 10 },
        { "plus-only", "+", 10 },
        { "double-sign", "+-5", 10 },
        { "leading-space", " 42", 10 },
        { "trailing-space", "42 ", 10 },
        { "trailing-newline", "42\n", 10 },
        { "trailing-garbage", "42px", 10 },
        { "underscore", "1_000", 10 },
        { "thousands-separator", "1,000", 10 },
        { "decimal-point", "4.0", 10 },
        { "exponent", "1e3", 10 },
        { "hex-prefix", "0x1A", 16 },
        { "binary-prefix", "0b101", 2 },
        { "digit-outside-base", "102", 2 },
        { "letter-outside-base", "fg", 16 },
        { "arabic-indic-digits", "\u0661\u0662", 10 },
        { "fullwidth-digits", "\uff11\uff12", 10 },
        { "kelvin-sign", "\u212a", 36 },
    };

    [Theory]
    [MemberData(nameof(Invalid))]
    public void RejectsInvalid(string name, string s, int radix)
    {
        Assert.NotNull(name);
        Assert.Throws<FormatException>(() => IntegerParser.ParseInteger(s, radix));
    }

    public static TheoryData<string, string, int> OutOfRange => new()
    {
        { "max-plus-one", "9223372036854775808", 10 },
        { "min-minus-one", "-9223372036854775809", 10 },
        { "two-to-the-64", "18446744073709551616", 10 },
        { "hex-sign-bit", "8000000000000000", 16 },
        { "hex-all-ones", "ffffffffffffffff", 16 },
        { "min-times-ten", "-92233720368547758080", 10 },
        { "many-digits", new string('9', 5000), 10 },
    };

    [Theory]
    [MemberData(nameof(OutOfRange))]
    public void RejectsOutOfRange(string name, string s, int radix)
    {
        Assert.NotNull(name);
        Assert.Throws<OverflowException>(() => IntegerParser.ParseInteger(s, radix));
    }
}
