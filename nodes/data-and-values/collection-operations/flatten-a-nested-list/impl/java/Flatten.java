// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

import java.util.Arrays;

final class Flatten {
  private Flatten() {}

  /** Returns the items of the inner arrays, one level deep, in order. */
  static int[] flatten(int[][] lists) {
    return Arrays.stream(lists).flatMapToInt(Arrays::stream).toArray();
  }
}
