// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

/// Returns a + b, a - b, a * b, a / b (rounded toward zero), a % b (with the
/// sign of a) and a to the power n. b must not be 0.
pub fn arith(a: i64, b: i64, n: u32) -> [i64; 6] {
    // i64::pow is an exact integer power. ^ is the bitwise exclusive or.
    [a + b, a - b, a * b, a / b, a % b, a.pow(n)]
}
