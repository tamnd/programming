// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

/// Returns the sum of the items as an i64, using a for loop.
pub fn total(items: &[i32]) -> i64 {
    let mut s: i64 = 0;
    // &x copies each i32 out of the slice.
    for &x in items {
        s += i64::from(x);
    }
    s
}
