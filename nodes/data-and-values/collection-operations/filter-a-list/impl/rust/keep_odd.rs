// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

/// Returns a new vector of the odd items, in their order.
pub fn keep_odd(items: &[i32]) -> Vec<i32> {
    // Use != 0, not == 1: the remainder of a negative item can be negative.
    items.iter().copied().filter(|x| x % 2 != 0).collect()
}
