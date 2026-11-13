// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

/// Returns the chunks of items with size items each; the last chunk can be shorter.
/// Panics if size is 0.
pub fn chunks(items: &[i32], size: usize) -> Vec<Vec<i32>> {
    items.chunks(size).map(<[i32]>::to_vec).collect()
}
