// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.

/// Rounds `x` to the nearest integer; a half goes away from zero.
/// `f64::round` has this rule; `f64::round_ties_even` rounds a half to the
/// even integer. The contract keeps `x` in the range of `i64`.
pub fn round_half_away(x: f64) -> i64 {
    x.round() as i64
}
