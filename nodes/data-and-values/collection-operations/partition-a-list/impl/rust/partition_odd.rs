// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

/// Returns the odd items and the even items, each in input order.
pub fn partition_odd(items: &[i32]) -> (Vec<i32>, Vec<i32>) {
    // copied() gives i32 values, not references. `x % 2 != 0` is true for -3.
    items.iter().copied().partition(|x| x % 2 != 0)
}
