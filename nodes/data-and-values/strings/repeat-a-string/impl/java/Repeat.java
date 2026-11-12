// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

/** Repeats a string. */
public final class Repeat {
  private Repeat() {}

  /** Returns n copies of s. String.repeat throws if n is negative. */
  public static String repeat(String s, int n) {
    return s.repeat(n);
  }
}
