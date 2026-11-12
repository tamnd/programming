// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

/** Trims the ASCII white space from a string. */
public final class Trim {
  private Trim() {}

  // trim() removes each char up to U+0020, and strip() removes the white space
  // of Character.isWhitespace, so the class uses its own set.
  private static boolean isSpace(char c) {
    return c == ' ' || (c >= '\t' && c <= '\r');
  }

  /** Returns s with the white space at the start and at the end removed. */
  public static String trim(String s) {
    int start = 0;
    int end = s.length();
    while (start < end && isSpace(s.charAt(start))) {
      start++;
    }
    while (end > start && isSpace(s.charAt(end - 1))) {
      end--;
    }
    return s.substring(start, end);
  }

  /** Returns s with the white space at the start removed. */
  public static String trimStart(String s) {
    int start = 0;
    while (start < s.length() && isSpace(s.charAt(start))) {
      start++;
    }
    return s.substring(start);
  }
}
