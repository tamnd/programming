// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

/// Returns a new vector of items[start] up to, but not including, items[stop].
///
/// # Panics
///
/// Panics if start > stop or stop > items.len().
pub fn slice(items: &[i32], start: usize, stop: usize) -> Vec<i32> {
    // &items[start..stop] borrows the items; to_vec copies them.
    items[start..stop].to_vec()
}
