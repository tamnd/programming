// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

/// Returns a / b rounded toward zero, or None when b is 0 or when the
/// quotient does not fit in an i64.
pub fn safe_div(a: i64, b: i64) -> Option<i64> {
    // a / b panics in both cases. checked_div returns None instead.
    a.checked_div(b)
}
