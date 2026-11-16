// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.

final class RoundHalfAway {
  private RoundHalfAway() {}

  /**
   * Rounds x to the nearest integer; a half goes away from zero. Math.round rounds a half toward
   * positive infinity, so Math.round(-2.5) is -2. Round the magnitude, then set the sign.
   */
  static long roundHalfAway(double x) {
    long r = Math.round(Math.abs(x));
    return x < 0 ? -r : r;
  }
}
