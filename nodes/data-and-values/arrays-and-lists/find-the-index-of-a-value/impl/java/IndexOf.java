// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

final class IndexOf {
  private IndexOf() {}

  /** Returns the first index of value in items, or -1 if it is not there. */
  static int indexOf(int[] items, int value) {
    // Arrays.binarySearch needs a sorted array, so use a loop.
    for (int i = 0; i < items.length; i++) {
      if (items[i] == value) {
        return i;
      }
    }
    return -1;
  }
}
