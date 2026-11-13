// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

final class IndexOf {
  private IndexOf() {}

  /** Returns the first index of target in the sorted array items, or -1. */
  static int indexOf(int[] items, int target) {
    // Arrays.binarySearch can return any index of equal items, so this is a
    // search for the first index lo with items[lo] >= target.
    int lo = 0;
    int hi = items.length;
    while (lo < hi) {
      int mid = lo + (hi - lo) / 2;
      if (items[mid] < target) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }
    return lo < items.length && items[lo] == target ? lo : -1;
  }
}
