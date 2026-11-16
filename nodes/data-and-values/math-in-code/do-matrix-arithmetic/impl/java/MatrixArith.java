// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

/** Matrix arithmetic on arrays of rows. Java has no matrix type in the standard library. */
public final class MatrixArith {
    private MatrixArith() {}

    /** Returns the transpose of m: row i of the result is column i of m. */
    public static long[][] transpose(long[][] m) {
        long[][] t = new long[m[0].length][m.length];
        for (int i = 0; i < m.length; i++) {
            for (int j = 0; j < m[0].length; j++) {
                t[j][i] = m[i][j];
            }
        }
        return t;
    }

    /** Returns the product a times b. The number of columns of a must be the number of rows of b. */
    public static long[][] multiply(long[][] a, long[][] b) {
        int n = a.length;
        int m = b.length;
        int p = b[0].length;
        if (a[0].length != m) {
            throw new IllegalArgumentException("the sizes do not match");
        }
        long[][] c = new long[n][p];
        // The order i, k, j reads b row by row, which is faster than column by column.
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < m; k++) {
                long x = a[i][k];
                for (int j = 0; j < p; j++) {
                    c[i][j] += x * b[k][j];
                }
            }
        }
        return c;
    }
}
