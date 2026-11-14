// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

/// Returns the first index of value in items, or -1 if it is not there.
///
/// Rust code would often return the `Option<usize>` from `position` as it
/// is. This function gives -1 to match the contract of the task.
pub fn index_of(items: &[i32], value: i32) -> i64 {
    match items.iter().position(|&x| x == value) {
        Some(i) => i as i64,
        None => -1,
    }
}
