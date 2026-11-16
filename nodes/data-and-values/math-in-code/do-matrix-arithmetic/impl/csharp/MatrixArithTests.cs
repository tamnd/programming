// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

using System;
using Xunit;

public sealed class MatrixArithTests
{
    public static TheoryData<string, long[][], long[][], long[][]> Cases => new()
    {
        { "two-by-two", new long[][] { new long[] { 1, 2 }, new long[] { 3, 4 } }, new long[][] { new long[] { 5, 6 }, new long[] { 7, 8 } }, new long[][] { new long[] { 19, 22 }, new long[] { 43, 50 } } },
        { "two-by-three-times-three-by-two", new long[][] { new long[] { 1, 2, 3 }, new long[] { 4, 5, 6 } }, new long[][] { new long[] { 7, 8 }, new long[] { 9, 10 }, new long[] { 11, 12 } }, new long[][] { new long[] { 58, 64 }, new long[] { 139, 154 } } },
        { "one-by-one", new long[][] { new long[] { 3 } }, new long[][] { new long[] { 4 } }, new long[][] { new long[] { 12 } } },
        { "row-times-column", new long[][] { new long[] { 1, 2, 3 } }, new long[][] { new long[] { 4 }, new long[] { 5 }, new long[] { 6 } }, new long[][] { new long[] { 32 } } },
        { "column-times-row", new long[][] { new long[] { 1 }, new long[] { 2 }, new long[] { 3 } }, new long[][] { new long[] { 4, 5, 6 } }, new long[][] { new long[] { 4, 5, 6 }, new long[] { 8, 10, 12 }, new long[] { 12, 15, 18 } } },
        { "identity", new long[][] { new long[] { 1, 0 }, new long[] { 0, 1 } }, new long[][] { new long[] { 5, 6 }, new long[] { 7, 8 } }, new long[][] { new long[] { 5, 6 }, new long[] { 7, 8 } } },
        { "other-order", new long[][] { new long[] { 5, 6 }, new long[] { 7, 8 } }, new long[][] { new long[] { 1, 2 }, new long[] { 3, 4 } }, new long[][] { new long[] { 23, 34 }, new long[] { 31, 46 } } },
        { "zero-matrix", new long[][] { new long[] { 0, 0 }, new long[] { 0, 0 } }, new long[][] { new long[] { 1, 2 }, new long[] { 3, 4 } }, new long[][] { new long[] { 0, 0 }, new long[] { 0, 0 } } },
        { "negative-numbers", new long[][] { new long[] { -1, 2 }, new long[] { 3, -4 } }, new long[][] { new long[] { 2, 0 }, new long[] { 1, -1 } }, new long[][] { new long[] { 0, -2 }, new long[] { 2, 4 } } },
        { "beyond-32-bit", new long[][] { new long[] { 100000, 0 }, new long[] { 0, 100000 } }, new long[][] { new long[] { 100000 }, new long[] { 3 } }, new long[][] { new long[] { 10000000000 }, new long[] { 300000 } } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Multiply(string name, long[][] a, long[][] b, long[][] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, MatrixArith.Multiply(a, b));
    }

    [Fact]
    public void Transpose()
    {
        long[][] m = [[1, 2, 3], [4, 5, 6]];
        long[][] want = [[1, 4], [2, 5], [3, 6]];
        Assert.Equal(want, MatrixArith.Transpose(m));
    }

    [Fact]
    public void SizesMustMatch()
    {
        Assert.Throws<ArgumentException>(() => MatrixArith.Multiply([[1, 2]], [[1, 2]]));
    }
}
