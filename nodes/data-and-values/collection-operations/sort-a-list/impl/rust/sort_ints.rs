// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

/// Returns a new vector of the items in ascending order.
pub fn sort_ints(items: &[i32]) -> Vec<i32> {
    let mut out = items.to_vec();
    // For integers, equal items are the same, so an unstable sort is fine.
    out.sort_unstable();
    out
}
