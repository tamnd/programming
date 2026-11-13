// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

/// Returns a new vector with the items of a, then the items of b.
pub fn concat(a: &[i32], b: &[i32]) -> Vec<i32> {
    // concat on an array of slices copies the items into a new vector.
    [a, b].concat()
}
