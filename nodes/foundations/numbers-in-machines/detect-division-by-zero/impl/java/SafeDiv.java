// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.
import java.util.OptionalLong;

final class SafeDiv {
  private SafeDiv() {}

  /**
   * Returns a / b rounded toward zero, or an empty OptionalLong for a zero divisor or a quotient
   * that does not fit in a long.
   */
  static OptionalLong safeDiv(long a, long b) {
    try {
      // a / b throws ArithmeticException when b is 0, but Long.MIN_VALUE / -1 gives
      // Long.MIN_VALUE with no exception. Math.divideExact throws in both cases.
      return OptionalLong.of(Math.divideExact(a, b));
    } catch (ArithmeticException e) {
      return OptionalLong.empty();
    }
  }
}
