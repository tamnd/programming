// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

/// Returns true if some item of items is equal to value.
pub fn contains(items: &[i32], value: i32) -> bool {
    // contains takes a reference to the value.
    items.contains(&value)
}
