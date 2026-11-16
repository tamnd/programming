// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.
import java.util.OptionalLong;

final class CheckedAdd {
  private CheckedAdd() {}

  /** Returns a + b, or an empty OptionalLong when the sum does not fit in a long. */
  static OptionalLong checkedAdd(long a, long b) {
    try {
      // Math.addExact throws ArithmeticException on overflow; a + b wraps.
      return OptionalLong.of(Math.addExact(a, b));
    } catch (ArithmeticException e) {
      return OptionalLong.empty();
    }
  }

  /** The same check with a test before the addition, with no exception. */
  static OptionalLong checkedAddByTest(long a, long b) {
    if ((b > 0 && a > Long.MAX_VALUE - b) || (b < 0 && a < Long.MIN_VALUE - b)) {
      return OptionalLong.empty();
    }
    return OptionalLong.of(a + b);
  }
}
