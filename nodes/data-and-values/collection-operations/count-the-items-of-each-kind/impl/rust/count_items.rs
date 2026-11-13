// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

use std::collections::BTreeMap;

/// Returns one (item, count) pair for each distinct item, in ascending order of the item.
pub fn count_items(items: &[i32]) -> Vec<(i32, usize)> {
    // A BTreeMap gives its keys in ascending order.
    let mut counts = BTreeMap::new();
    for &x in items {
        *counts.entry(x).or_insert(0) += 1;
    }
    counts.into_iter().collect()
}
