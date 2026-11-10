// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

/** Parses a signed 64-bit integer with a strict syntax. */
public final class ParseInteger {
  private ParseInteger() {}

  /**
   * Parses s as a signed 64-bit integer in the given radix, from 2 to 36.
   *
   * @throws NumberFormatException if s is not an optional sign and ASCII digits of the radix
   * @throws ArithmeticException if the value does not fit in a long
   */
  public static long parseInteger(String s, int radix) {
    int start = s.startsWith("+") || s.startsWith("-") ? 1 : 0;
    if (start == s.length()) {
      throw invalid(s, radix);
    }
    for (int i = start; i < s.length(); i++) {
      char c = s.charAt(i);
      // Character.digit also accepts digits outside ASCII, such as U+0661.
      if (c > 0x7f || Character.digit(c, radix) < 0) {
        throw invalid(s, radix);
      }
    }
    try {
      return Long.parseLong(s, radix);
    } catch (NumberFormatException e) {
      // The syntax is valid, so the only cause left is a value out of range.
      throw new ArithmeticException("integer out of range: \"" + s + "\"");
    }
  }

  private static NumberFormatException invalid(String s, int radix) {
    return new NumberFormatException("invalid integer for radix " + radix + ": \"" + s + "\"");
  }
}
