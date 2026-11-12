// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

/** Changes the case of the ASCII letters of a string. */
public final class Case {
  private Case() {}

  /**
   * Returns s with a to z changed to A to Z. toUpperCase() uses the default
   * locale, and toUpperCase(Locale.ROOT) changes the letters of Unicode, so the
   * loop changes only the ASCII letters.
   */
  public static String toUpper(String s) {
    char[] cs = s.toCharArray();
    for (int i = 0; i < cs.length; i++) {
      if (cs[i] >= 'a' && cs[i] <= 'z') {
        cs[i] = (char) (cs[i] - 'a' + 'A');
      }
    }
    return new String(cs);
  }

  /** Returns s with A to Z changed to a to z. */
  public static String toLower(String s) {
    char[] cs = s.toCharArray();
    for (int i = 0; i < cs.length; i++) {
      if (cs[i] >= 'A' && cs[i] <= 'Z') {
        cs[i] = (char) (cs[i] - 'A' + 'a');
      }
    }
    return new String(cs);
  }
}
