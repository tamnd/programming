// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

import java.util.Arrays;

final class Filled {
  private Filled() {}

  /** Returns a new array of n items, and each item is value. */
  static int[] filled(int n, int value) {
    // new int[n] holds n zeros. fill then sets each item.
    int[] a = new int[n];
    Arrays.fill(a, value);
    return a;
  }
}
