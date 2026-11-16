// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

/// Returns n! for 0 <= n <= 20. The empty range 1..=0 gives 1.
pub fn factorial(n: i64) -> i64 {
    assert!(n >= 0, "n is negative");
    (1..=n).product()
}

/// Returns None if n! does not fit in an i64.
pub fn checked_factorial(n: i64) -> Option<i64> {
    (1..=n).try_fold(1i64, |f, i| f.checked_mul(i))
}
