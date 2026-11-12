// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

/// Returns the items with sep between each two.
pub fn join(items: &[&str], sep: &str) -> String {
    // join on a slice of string slices makes one String of the right size.
    items.join(sep)
}

/// Returns the decimal form of each item, with sep between each two.
pub fn join_numbers(items: &[i32], sep: &str) -> String {
    // join needs items that can be borrowed as str, so the code converts each
    // number first.
    items.iter().map(i32::to_string).collect::<Vec<_>>().join(sep)
}
