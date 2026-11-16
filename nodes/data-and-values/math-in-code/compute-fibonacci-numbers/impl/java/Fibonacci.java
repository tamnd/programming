// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

import java.math.BigInteger;

/** Fibonacci numbers, with F(0) = 0 and F(1) = 1. */
public final class Fibonacci {
    private Fibonacci() {}

    /** Returns F(n). addExact throws ArithmeticException above F(92). */
    public static long fibonacci(long n) {
        if (n < 0) {
            throw new IllegalArgumentException("n is negative");
        }
        if (n == 0) {
            return 0;
        }
        long a = 0;
        long b = 1;
        for (long i = 1; i < n; i++) {
            long next = Math.addExact(a, b);
            a = b;
            b = next;
        }
        return b;
    }

    /** Returns F(n) as a BigInteger, with no size limit. */
    public static BigInteger bigFibonacci(int n) {
        BigInteger a = BigInteger.ZERO;
        BigInteger b = BigInteger.ONE;
        for (int i = 0; i < n; i++) {
            BigInteger next = a.add(b);
            a = b;
            b = next;
        }
        return a;
    }
}
