// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

/// Returns a new vector of the items in the opposite order.
pub fn reverse(items: &[i32]) -> Vec<i32> {
    // rev() walks the items from the end; copied() turns each &i32 into an i32.
    items.iter().rev().copied().collect()
}
