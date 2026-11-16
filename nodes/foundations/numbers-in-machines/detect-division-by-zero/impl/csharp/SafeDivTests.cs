// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.
using System;
using Xunit;

public sealed class SafeDivTests
{
    private const long Max = long.MaxValue;
    private const long Min = long.MinValue;

    public static TheoryData<string, long, long, long?> Cases => new()
    {
        { "exact", 12, 4, 3 },
        { "remainder-dropped", 7, 2, 3 },
        { "zero-dividend", 0, 5, 0 },
        { "zero-divisor", 5, 0, null },
        { "negative-by-zero", -5, 0, null },
        { "zero-by-zero", 0, 0, null },
        { "negative-dividend", -7, 2, -3 },
        { "negative-divisor", 7, -2, -3 },
        { "both-negative", -7, -2, 3 },
        { "min-by-minus-one", Min, -1, null },
        { "min-by-one", Min, 1, Min },
        { "max-by-minus-one", Max, -1, -Max },
        { "min-by-max", Min, Max, -1 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, long a, long b, long? want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Division.SafeDiv(a, b));
    }

    [Fact]
    public void ZeroDivisorThrows()
    {
        long zero = 0;
        Assert.Throws<DivideByZeroException>(() => 5 / zero);
    }
}
