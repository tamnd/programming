// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

import java.math.BigInteger;

/** The factorial of a non-negative integer. */
public final class Factorial {
    private Factorial() {}

    /** Returns n!. multiplyExact throws ArithmeticException above 20!. */
    public static long factorial(long n) {
        if (n < 0) {
            throw new IllegalArgumentException("n is negative");
        }
        long f = 1;
        for (long i = 2; i <= n; i++) {
            f = Math.multiplyExact(f, i);
        }
        return f;
    }

    /** Returns n! as a BigInteger, with no size limit. */
    public static BigInteger bigFactorial(int n) {
        BigInteger f = BigInteger.ONE;
        for (int i = 2; i <= n; i++) {
            f = f.multiply(BigInteger.valueOf(i));
        }
        return f;
    }
}
