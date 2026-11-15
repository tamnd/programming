// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

/**
 * Returns [row, column] of the first cell that equals target, or [-1, -1].
 * @param {number[][]} grid
 * @param {number} target
 * @returns {[number, number]}
 */
export function findInGrid(grid, target) {
  let found = [-1, -1];
  outer: for (let r = 0; r < grid.length; r++) {
    for (let c = 0; c < grid[r].length; c++) {
      if (grid[r][c] === target) {
        found = [r, c];
        // The label makes break leave both loops.
        break outer;
      }
    }
  }
  return found;
}
