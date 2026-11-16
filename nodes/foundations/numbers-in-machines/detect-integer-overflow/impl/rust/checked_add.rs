// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

/// Returns a + b, or None when the sum does not fit in an i64.
pub fn checked_add(a: i64, b: i64) -> Option<i64> {
    // a + b panics on overflow in a debug build and wraps in a release build.
    // checked_add returns None in both.
    a.checked_add(b)
}
