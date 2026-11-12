// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

/** Compares strings in the order of their code points. */
public final class Compare {
  private Compare() {}

  /**
   * Returns -1, 0 or 1 for the code point order of a and b. compareTo compares
   * UTF-16 code units, so the loop compares code points.
   */
  public static int compare(String a, String b) {
    int i = 0;
    while (i < a.length() && i < b.length()) {
      int x = a.codePointAt(i);
      int y = b.codePointAt(i);
      if (x != y) {
        return x < y ? -1 : 1;
      }
      i += Character.charCount(x);
    }
    return Integer.signum(a.length() - b.length());
  }

  /** Returns true if a and b have the same code points. */
  public static boolean equals(String a, String b) {
    return a.equals(b);
  }
}
