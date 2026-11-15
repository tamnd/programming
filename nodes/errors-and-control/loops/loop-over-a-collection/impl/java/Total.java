// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

final class Total {
  private Total() {}

  /** Returns the sum of the items, using an enhanced for loop. */
  static long total(int[] items) {
    // A long sum: an int sum wraps around past Integer.MAX_VALUE.
    long s = 0;
    for (int x : items) {
      s += x;
    }
    return s;
  }
}
