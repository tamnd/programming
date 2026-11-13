// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

/// Returns the pairs (a[i], b[i]) up to the end of the shorter slice.
pub fn zip_pairs(a: &[i32], b: &[i32]) -> Vec<(i32, i32)> {
    // zip stops when one of the two iterators has no more items.
    a.iter().copied().zip(b.iter().copied()).collect()
}
