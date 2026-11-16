// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

/// Returns the quotient and the remainder of the floor division of `a` by `b`.
pub fn div_mod(a: i32, b: i32) -> (i32, i32) {
    // The operators / and % truncate toward zero. Move a result that is
    // not exact one step down, so that the quotient is the floor.
    let (q, r) = (a / b, a % b);
    if r != 0 && (r < 0) != (b < 0) {
        (q - 1, r + b)
    } else {
        (q, r)
    }
}
