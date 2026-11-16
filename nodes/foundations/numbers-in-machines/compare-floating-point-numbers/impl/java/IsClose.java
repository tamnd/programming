// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

final class IsClose {
  private IsClose() {}

  /**
   * Tells whether a and b are close, by the rule of math.isclose in Python: |a - b| <= max(relTol *
   * max(|a|, |b|), absTol). NaN is close to no value, and an infinity is close only to itself.
   */
  static boolean isClose(double a, double b, double relTol, double absTol) {
    if (a == b) {
      return true; // also an infinity and itself
    }
    if (!Double.isFinite(a) || !Double.isFinite(b)) {
      return false; // NaN or an infinity
    }
    double diff = Math.abs(a - b);
    return diff <= Math.max(relTol * Math.max(Math.abs(a), Math.abs(b)), absTol);
  }
}
