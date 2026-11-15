// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

final class CountTo {
  private CountTo() {}

  /** Returns the array {1, 2, ..., n}. */
  static int[] countTo(int n) {
    int[] out = new int[n];
    for (int i = 1; i <= n; i++) {
      out[i - 1] = i;
    }
    return out;
  }
}
