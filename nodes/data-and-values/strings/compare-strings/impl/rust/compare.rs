// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

use std::cmp::Ordering;

/// Returns -1, 0 or 1. The order of str compares the UTF-8 bytes, which is the
/// order of the code points.
pub fn compare(a: &str, b: &str) -> i32 {
    match a.cmp(b) {
        Ordering::Less => -1,
        Ordering::Equal => 0,
        Ordering::Greater => 1,
    }
}

/// Returns true if a and b have the same code points.
pub fn equals(a: &str, b: &str) -> bool {
    a == b
}
