// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

/** Gets a substring by code point indexes. */
public final class Substring {
  private Substring() {}

  /** Returns the code points of s from index start up to, not including, end. */
  public static String substring(String s, int start, int end) {
    // String.substring takes char indexes, which are UTF-16 code units, so
    // the code converts the code point indexes first.
    int from = s.offsetByCodePoints(0, start);
    int to = s.offsetByCodePoints(from, end - start);
    return s.substring(from, to);
  }
}
