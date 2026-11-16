// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

/// Returns the greatest common divisor and the least common multiple of a
/// and b. unsigned_abs gives a u64, so the loop has no signs.
///
/// # Panics
///
/// Panics if a result does not fit in an i64.
pub fn gcd_and_lcm(a: i64, b: i64) -> (i64, i64) {
    let (mut x, mut y) = (a.unsigned_abs(), b.unsigned_abs());
    while y != 0 {
        (x, y) = (y, x % y);
    }
    let lcm = if a == 0 || b == 0 {
        0
    } else {
        (a.unsigned_abs() / x)
            .checked_mul(b.unsigned_abs())
            .expect("the lcm does not fit")
    };
    (
        i64::try_from(x).expect("the gcd does not fit"),
        i64::try_from(lcm).expect("the lcm does not fit"),
    )
}
