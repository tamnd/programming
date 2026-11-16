// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.
final class Arith {
  private Arith() {}

  /**
   * Returns the sum, the difference, the product, the quotient rounded toward zero, the remainder
   * with the sign of a, and a to the power n.
   */
  static long[] arith(long a, long b, int n) {
    // Java has no power operator, and Math.pow works on double values. A loop keeps the power
    // exact. ^ is not a power: it is the bitwise exclusive or.
    long power = 1;
    for (int i = 0; i < n; i++) {
      power *= a;
    }
    return new long[] {a + b, a - b, a * b, a / b, a % b, power};
  }
}
