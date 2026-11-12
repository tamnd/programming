// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

/** Checks whether a string starts or ends with another string. */
public final class Prefix {
  private Prefix() {}

  /** Returns true if s starts with prefix. */
  public static boolean startsWith(String s, String prefix) {
    return s.startsWith(prefix);
  }

  /** Returns true if s ends with suffix. */
  public static boolean endsWith(String s, String suffix) {
    return s.endsWith(suffix);
  }
}
