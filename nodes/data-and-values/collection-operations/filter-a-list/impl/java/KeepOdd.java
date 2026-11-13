// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

import java.util.Arrays;

final class KeepOdd {
  private KeepOdd() {}

  /** Returns a new array of the odd items, in their order. */
  static int[] keepOdd(int[] items) {
    // Use != 0, not == 1: the remainder of a negative item can be negative.
    return Arrays.stream(items).filter(x -> x % 2 != 0).toArray();
  }
}
