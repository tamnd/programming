// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

/// Returns rows vectors of cols items, each item value.
pub fn grid(rows: usize, cols: usize, value: i32) -> Vec<Vec<i32>> {
    // vec! clones the inner vector for each row, so the rows do not share storage.
    vec![vec![value; cols]; rows]
}
