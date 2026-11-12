// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

/// Returns the code point of the one character in `c`. `c.as_bytes()[0]`
/// would give only the first byte of the UTF-8 sequence.
pub fn code_point(c: &str) -> u32 {
    c.chars().next().map_or(0, u32::from)
}
