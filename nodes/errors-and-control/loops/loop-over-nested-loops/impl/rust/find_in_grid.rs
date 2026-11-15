// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

/// Returns (row, column) of the first cell that equals target, or (-1, -1).
pub fn find_in_grid(grid: &[Vec<i32>], target: i32) -> (isize, isize) {
    let mut found = (-1, -1);
    'outer: for (r, row) in grid.iter().enumerate() {
        for (c, &x) in row.iter().enumerate() {
            if x == target {
                found = (r as isize, c as isize);
                // The label makes break leave both loops.
                break 'outer;
            }
        }
    }
    found
}
