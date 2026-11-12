// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

/** Concatenates two strings, and a string and a number. */
public final class Concatenate {
  private Concatenate() {}

  /** Returns the characters of a followed by the characters of b. */
  public static String concat(String a, String b) {
    return a + b;
  }

  /** Returns s followed by the decimal form of n. */
  public static String concatNumber(String s, int n) {
    // One String operand makes + a string concatenation, so n is converted.
    return s + n;
  }
}
