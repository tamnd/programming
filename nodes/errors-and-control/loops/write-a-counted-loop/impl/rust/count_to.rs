// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

/// Returns the vector [1, 2, ..., n].
pub fn count_to(n: i32) -> Vec<i32> {
    let mut out = Vec::new();
    // 1..=n includes n; 1..n would stop before it.
    for i in 1..=n {
        out.push(i);
    }
    out
}
