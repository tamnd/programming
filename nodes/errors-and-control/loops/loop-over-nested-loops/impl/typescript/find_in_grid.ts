// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

export function findInGrid(grid: readonly (readonly number[])[], target: number): [number, number] {
  let found: [number, number] = [-1, -1];
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
