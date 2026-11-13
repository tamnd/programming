// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

/// Returns a new vector with the first `k` items moved to the end.
pub fn rotate_left(items: &[i32], k: usize) -> Vec<i32> {
    let mut v = items.to_vec();
    if !v.is_empty() {
        // slice::rotate_left panics if its argument is larger than the length.
        let s = k % v.len();
        v.rotate_left(s);
    }
    v
}
