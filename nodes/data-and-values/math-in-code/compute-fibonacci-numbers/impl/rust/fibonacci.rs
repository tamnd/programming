// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

/// Returns F(n), or None if F(n) does not fit in an i64.
pub fn checked_fibonacci(n: u32) -> Option<i64> {
    if n == 0 {
        return Some(0);
    }
    let (mut a, mut b) = (0i64, 1i64);
    for _ in 1..n {
        let next = a.checked_add(b)?;
        a = b;
        b = next;
    }
    Some(b)
}

/// Returns F(n) for 0 <= n <= 92.
pub fn fibonacci(n: i64) -> i64 {
    let n = u32::try_from(n).expect("n is negative or too large");
    checked_fibonacci(n).expect("F(n) does not fit in an i64")
}
