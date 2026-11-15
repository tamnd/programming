// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

final class FindInGrid {
  private FindInGrid() {}

  /** Returns {row, column} of the first cell that equals target, or {-1, -1}. */
  static int[] findInGrid(int[][] grid, int target) {
    int[] found = {-1, -1};
    outer:
    for (int r = 0; r < grid.length; r++) {
      for (int c = 0; c < grid[r].length; c++) {
        if (grid[r][c] == target) {
          found = new int[] {r, c};
          // The label makes break leave both loops.
          break outer;
        }
      }
    }
    return found;
  }
}
