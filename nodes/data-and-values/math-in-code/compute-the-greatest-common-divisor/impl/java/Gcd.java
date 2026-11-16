// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

/** The greatest common divisor and the least common multiple of two long values. */
public final class Gcd {
    private Gcd() {}

    /** Returns {gcd, lcm}. absExact throws ArithmeticException for Long.MIN_VALUE. */
    public static long[] gcdAndLcm(long a, long b) {
        long x = Math.absExact(a);
        long y = Math.absExact(b);
        while (y != 0) {
            long r = x % y;
            x = y;
            y = r;
        }
        long lcm = a == 0 || b == 0 ? 0 : Math.multiplyExact(Math.abs(a) / x, Math.abs(b));
        return new long[] {x, lcm};
    }
}
