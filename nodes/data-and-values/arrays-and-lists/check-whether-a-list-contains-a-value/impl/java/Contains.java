// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

import java.util.Arrays;

final class Contains {
  private Contains() {}

  /** Returns true if some item of items is equal to value. */
  static boolean contains(int[] items, int value) {
    // Arrays.asList(items) would be a List<int[]> with one item, so use a stream of int.
    return Arrays.stream(items).anyMatch(x -> x == value);
  }
}
