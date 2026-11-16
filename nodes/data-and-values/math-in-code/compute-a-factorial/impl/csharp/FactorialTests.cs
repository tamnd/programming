// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

using System;
using Xunit;

public sealed class FactorialTests
{
    public static TheoryData<string, long, long> Cases => new()
    {
        { "five", 5, 120 },
        { "ten", 10, 3628800 },
        { "zero", 0, 1 },
        { "one", 1, 1 },
        { "largest-32-bit", 12, 479001600 },
        { "beyond-32-bit", 13, 6227020800 },
        { "largest-64-bit", 20, 2432902008176640000 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Compute(string name, long n, long want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Factorial.Compute(n));
    }

    [Fact]
    public void OverflowThrows()
    {
        Assert.Throws<OverflowException>(() => Factorial.Compute(21));
        Assert.Throws<ArgumentOutOfRangeException>(() => Factorial.Compute(-1));
    }
}
