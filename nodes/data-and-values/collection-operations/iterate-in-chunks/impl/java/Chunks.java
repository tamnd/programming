// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

import java.util.Arrays;

final class Chunks {
  private Chunks() {}

  /** Returns the chunks of items with size items each; the last chunk can be shorter. */
  static int[][] chunks(int[] items, int size) {
    int count = items.length / size + (items.length % size != 0 ? 1 : 0);
    int[][] out = new int[count][];
    for (int k = 0; k < count; k++) {
      int from = k * size;
      out[k] = Arrays.copyOfRange(items, from, Math.min(from + size, items.length));
    }
    return out;
  }
}
