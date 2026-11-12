// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

/** Removes each code point of a set from a string. */
public final class Remove {
  private Remove() {}

  /**
   * Returns s without the code points of chars. The loop uses code points, so a
   * surrogate pair is one character, and indexOf(int) finds a code point above
   * U+FFFF in chars.
   */
  public static String removeChars(String s, String chars) {
    StringBuilder sb = new StringBuilder(s.length());
    s.codePoints().filter(cp -> chars.indexOf(cp) < 0).forEach(sb::appendCodePoint);
    return sb.toString();
  }
}
