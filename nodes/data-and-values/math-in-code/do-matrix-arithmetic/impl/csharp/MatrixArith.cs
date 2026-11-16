// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

using System;

/// <summary>Matrix arithmetic on jagged arrays of rows.</summary>
public static class MatrixArith
{
    /// <summary>Returns the transpose of m: row i of the result is column i of m.</summary>
    public static long[][] Transpose(long[][] m)
    {
        var t = new long[m[0].Length][];
        for (int j = 0; j < t.Length; j++)
        {
            t[j] = new long[m.Length];
            for (int i = 0; i < m.Length; i++)
            {
                t[j][i] = m[i][j];
            }
        }
        return t;
    }

    /// <summary>Returns the product a times b. The number of columns of a must be the number of rows of b.</summary>
    public static long[][] Multiply(long[][] a, long[][] b)
    {
        if (a[0].Length != b.Length)
        {
            throw new ArgumentException("the sizes do not match");
        }
        var c = new long[a.Length][];
        for (int i = 0; i < a.Length; i++)
        {
            c[i] = new long[b[0].Length];
            for (int k = 0; k < b.Length; k++)
            {
                for (int j = 0; j < b[0].Length; j++)
                {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return c;
    }
}
