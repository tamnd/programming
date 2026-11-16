// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

using Xunit;

public sealed class FixedTests
{
    public static TheoryData<string, double, int, string> Cases => new()
    {
        { "pi-two", 3.14159, 2, "3.14" },
        { "pad-zeros", 1.0, 3, "1.000" },
        { "round-up", 1234.5678, 2, "1234.57" },
        { "zero-digits", 123.456, 0, "123" },
        { "float-error", 0.30000000000000004, 2, "0.30" },
        { "negative", -1.234, 1, "-1.2" },
        { "rounds-to-zero", 0.0049, 2, "0.00" },
        { "stored-below-2-675", 2.675, 2, "2.67" },
        { "stored-below-1-005", 1.005, 2, "1.00" },
        { "large", 1e+20, 2, "100000000000000000000.00" },
        { "many-digits", 0.1, 20, "0.10000000000000000555" },
        { "integer-value", 42.0, 2, "42.00" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Format(string name, double x, int digits, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Fixed.Format(x, digits));
    }
}
