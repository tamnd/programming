// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

/** Replaces a substring in a string. The search is literal. */
public final class Replace {
  private Replace() {}

  /** Returns s with each match of old replaced by nw. */
  public static String replace(String s, String old, String nw) {
    // replace takes literal text. replaceAll and replaceFirst take a regex.
    return s.replace(old, nw);
  }

  /** Returns s with the first match of old replaced by nw. */
  public static String replaceFirst(String s, String old, String nw) {
    int i = s.indexOf(old);
    if (i < 0) {
      return s;
    }
    return s.substring(0, i) + nw + s.substring(i + old.length());
  }
}
