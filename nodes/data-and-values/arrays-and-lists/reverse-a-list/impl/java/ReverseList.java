// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

final class ReverseList {
  private ReverseList() {}

  /** Returns a new array of the items in the opposite order. */
  static int[] reverse(int[] items) {
    int n = items.length;
    int[] out = new int[n];
    for (int i = 0; i < n; i++) {
      out[i] = items[n - 1 - i];
    }
    return out;
  }
}
