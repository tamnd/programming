// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

use std::num::ParseIntError;

/// Parses `s` as a signed 64-bit integer in the given radix, from 2 to 36.
///
/// The error kind is `Empty` or `InvalidDigit` when `s` is not an optional
/// sign and ASCII digits of the radix, and `PosOverflow` or `NegOverflow`
/// when the value does not fit in an `i64`.
///
/// # Panics
///
/// Panics when `radix` is not in the range from 2 to 36.
pub fn parse_integer(s: &str, radix: u32) -> Result<i64, ParseIntError> {
    i64::from_str_radix(s, radix)
}
