// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

use std::fmt::Write;

/// Returns the numbers 1 to n, separated by commas.
pub fn number_line(n: u32) -> String {
    // A String grows its buffer when it is full, so each push is amortized
    // O(1). write! formats the number into the buffer with no temporary String.
    let mut s = String::new();
    for i in 1..=n {
        if i > 1 {
            s.push(',');
        }
        write!(s, "{i}").expect("a write to a String does not fail");
    }
    s
}
