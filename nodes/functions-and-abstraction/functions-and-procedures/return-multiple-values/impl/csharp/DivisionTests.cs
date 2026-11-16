// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

using Xunit;

public sealed class DivisionTests
{
    public static TheoryData<string, int, int, int, int> Cases => new()
    {
        { "positive", 17, 5, 3, 2 },
        { "exact", 20, 4, 5, 0 },
        { "smaller-dividend", 3, 10, 0, 3 },
        { "negative-dividend", -17, 5, -4, 3 },
        { "negative-divisor", 17, -5, -4, -3 },
        { "both-negative", -17, -5, 3, -2 },
        { "exact-negative", -20, 4, -5, 0 },
        { "small-negative", -3, 10, -1, 7 },
        { "zero-dividend", 0, 7, 0, 0 },
        { "zero-negative-divisor", 0, -7, 0, 0 },
        { "divisor-one", 9, 1, 9, 0 },
        { "int-max", 2147483647, 2, 1073741823, 1 },
        { "int-min", -2147483648, 3, -715827883, 1 },
        { "min-by-max", -2147483648, 2147483647, -2, 2147483646 },
        { "max-by-min", 2147483647, -2147483648, -1, -1 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void DivMod(string name, int a, int b, int q, int r)
    {
        Assert.NotNull(name);
        var (quotient, remainder) = Division.DivMod(a, b);
        Assert.Equal(q, quotient);
        Assert.Equal(r, remainder);
        Assert.Equal(q, Division.DivMod(a, b).Quotient);
    }
}
