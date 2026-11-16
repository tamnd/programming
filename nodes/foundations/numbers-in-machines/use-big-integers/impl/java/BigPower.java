// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.
import java.math.BigInteger;

final class BigPower {
  private BigPower() {}

  /** Returns base to the power exp as decimal text. A BigInteger has no fixed width. */
  static String bigPower(int base, int exp) {
    return BigInteger.valueOf(base).pow(exp).toString();
  }
}
