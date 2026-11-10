// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

/// Returns the code points of `s` in the opposite order.
///
/// A `char` is a Unicode scalar value, so `chars().rev()` reverses code
/// points. A combining mark moves to the letter that was after it, because
/// the standard library has no grapheme iteration.
pub fn reverse_code_points(s: &str) -> String {
    s.chars().rev().collect()
}
