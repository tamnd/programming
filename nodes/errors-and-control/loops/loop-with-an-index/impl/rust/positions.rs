// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

/// Returns the index of each item that equals target.
pub fn positions(items: &[&str], target: &str) -> Vec<usize> {
    let mut out = Vec::new();
    // enumerate gives (index, item) pairs. It must come before any filter.
    for (i, x) in items.iter().enumerate() {
        if *x == target {
            out.push(i);
        }
    }
    out
}
