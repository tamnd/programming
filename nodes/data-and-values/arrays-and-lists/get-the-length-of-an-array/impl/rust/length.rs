// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

/// Returns the number of items in items.
pub fn length(items: &[&str]) -> usize {
    // len counts the items. str::len would count the bytes of one string.
    items.len()
}
