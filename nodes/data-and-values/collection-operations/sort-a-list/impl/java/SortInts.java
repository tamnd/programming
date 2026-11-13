// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

import java.util.Arrays;

final class SortInts {
  private SortInts() {}

  /** Returns a new array of the items in ascending order. */
  static int[] sortInts(int[] items) {
    int[] out = items.clone();
    Arrays.sort(out);
    return out;
  }
}
