// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

/// Returns the first index of `target` in the sorted slice `items`, or -1.
pub fn index_of(items: &[i32], target: i32) -> i64 {
    // binary_search can return any index of equal items. partition_point
    // gives the first index i with items[i] >= target.
    let i = items.partition_point(|&x| x < target);
    if i < items.len() && items[i] == target {
        i64::try_from(i).expect("index_of: the index does not fit in an i64")
    } else {
        -1
    }
}
