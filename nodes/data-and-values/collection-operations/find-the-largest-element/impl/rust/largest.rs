// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

/// Returns the largest item. It panics if `items` is empty.
pub fn largest(items: &[i32]) -> i32 {
    // max gives None for an empty slice.
    *items.iter().max().expect("largest: the slice is empty")
}
