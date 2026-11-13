// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

/// Returns a new vector with value at position index. index items.len() appends.
///
/// # Panics
///
/// Panics if index > items.len(), as Vec::insert does.
pub fn insert_at(items: &[i32], index: usize, value: i32) -> Vec<i32> {
    let mut out = Vec::with_capacity(items.len() + 1);
    out.extend_from_slice(items);
    out.insert(index, value);
    out
}
