// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

final class RotateLeft {
  private RotateLeft() {}

  /** Returns a new array with the first k items moved to the end. k must not be negative. */
  static int[] rotateLeft(int[] items, int k) {
    int n = items.length;
    int[] out = new int[n];
    if (n == 0) {
      return out;
    }
    int s = k % n;
    System.arraycopy(items, s, out, 0, n - s);
    System.arraycopy(items, 0, out, n - s, s);
    return out;
  }
}
