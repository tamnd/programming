// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

import java.util.Arrays;

final class Grid {
  private Grid() {}

  /** Returns rows arrays of cols items, each item value. No two rows share an array. */
  static int[][] grid(int rows, int cols, int value) {
    // new int[rows][cols] makes a separate array for each row, filled with 0.
    int[][] g = new int[rows][cols];
    for (int[] row : g) {
      Arrays.fill(row, value);
    }
    return g;
  }
}
