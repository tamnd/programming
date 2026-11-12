// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

/** Searches a string for a substring, with indexes in code points. */
public final class Search {
  private Search() {}

  /** Returns the code point index of the first match of sub in s, or -1. */
  public static int find(String s, String sub) {
    // indexOf counts UTF-16 code units, so convert the index to code points.
    int i = s.indexOf(sub);
    return i < 0 ? -1 : s.codePointCount(0, i);
  }

  /** Returns the code point index of the last match of sub in s, or -1. */
  public static int findLast(String s, String sub) {
    int i = s.lastIndexOf(sub);
    return i < 0 ? -1 : s.codePointCount(0, i);
  }
}
