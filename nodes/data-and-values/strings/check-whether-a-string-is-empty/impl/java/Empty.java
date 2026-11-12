// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

/** Checks whether a string is empty or has only ASCII white space. */
public final class Empty {
  private Empty() {}

  /** Returns true if s has no characters. */
  public static boolean isEmpty(String s) {
    return s.isEmpty();
  }

  /** Returns true if each character of s is ASCII white space. */
  public static boolean isBlank(String s) {
    // isBlank() uses Character.isWhitespace, which also accepts Unicode spaces.
    return s.chars().allMatch(c -> " \t\n\u000b\f\r".indexOf(c) >= 0);
  }
}
