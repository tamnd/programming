// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

using System;
using Xunit;

public sealed class FibonacciTests
{
    public static TheoryData<string, long, long> Cases => new()
    {
        { "ten", 10, 55 },
        { "twenty", 20, 6765 },
        { "thirty", 30, 832040 },
        { "zero", 0, 0 },
        { "one", 1, 1 },
        { "two", 2, 1 },
        { "largest-32-bit", 46, 1836311903 },
        { "beyond-32-bit", 47, 2971215073 },
        { "binet-fails", 71, 308061521170129 },
        { "largest-64-bit", 92, 7540113804746346429 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Compute(string name, long n, long want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Fibonacci.Compute(n));
    }

    [Fact]
    public void OverflowThrows()
    {
        Assert.Throws<OverflowException>(() => Fibonacci.Compute(93));
        Assert.Throws<ArgumentOutOfRangeException>(() => Fibonacci.Compute(-1));
    }
}
