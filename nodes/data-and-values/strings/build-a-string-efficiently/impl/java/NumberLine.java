// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

/** Builds the line of numbers 1 to n. */
public final class NumberLine {
  private NumberLine() {}

  /** Returns the numbers 1 to n, separated by commas. */
  public static String numberLine(int n) {
    // A StringBuilder appends to one buffer, so the loop does not copy the
    // whole result on each step.
    StringBuilder sb = new StringBuilder();
    for (int i = 1; i <= n; i++) {
      if (i > 1) {
        sb.append(',');
      }
      sb.append(i);
    }
    return sb.toString();
  }
}
