// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

/// Returns n, n / 2, n / 4, ... while the value is greater than 0.
pub fn halvings(mut n: i32) -> Vec<i32> {
    let mut out = Vec::new();
    while n > 0 {
        out.push(n);
        n /= 2;
    }
    out
}
