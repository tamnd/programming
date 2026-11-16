// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

/// Returns x with exactly digits digits after the decimal point. It rounds
/// the exact binary value of x.
pub fn fixed(x: f64, digits: usize) -> String {
    format!("{x:.digits$}")
}
