// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

using System;
using Xunit;

public sealed class SumProdTests
{
    public static TheoryData<string, long[], long, long> Cases => new()
    {
        { "small", new long[] { 1, 2, 3, 4 }, 10, 24 },
        { "mixed", new long[] { 3, -2, 5 }, 6, -30 },
        { "empty", Array.Empty<long>(), 0, 1 },
        { "single", new long[] { 7 }, 7, 7 },
        { "with-zero", new long[] { 5, 0, 9 }, 14, 0 },
        { "two-negatives", new long[] { -2, -3 }, -5, 6 },
        { "three-negatives", new long[] { -1, -2, -3 }, -6, -6 },
        { "ones", new long[] { 1, 1, 1 }, 3, 1 },
        { "large-product", new long[] { 3037000499, 3037000499 }, 6074000998, 9223372030926249001 },
        { "to-the-limit", new long[] { 9223372036854775806, 1 }, 9223372036854775807, 9223372036854775806 },
        { "beyond-double", new long[] { 9007199254740993 }, 9007199254740993, 9007199254740993 },
        { "min", new long[] { long.MinValue }, long.MinValue, long.MinValue },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void SumAndProduct(string name, long[] items, long sum, long product)
    {
        Assert.NotNull(name);
        Assert.Equal((sum, product), SumProd.SumAndProduct(items));
    }

    [Fact]
    public void OverflowThrows()
    {
        Assert.Throws<OverflowException>(() => SumProd.SumAndProduct([4294967296L, 4294967296L]));
    }
}
