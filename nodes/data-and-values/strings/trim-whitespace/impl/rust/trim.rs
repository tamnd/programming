// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

/// Reports whether c is one of the 6 ASCII white space characters.
/// char::is_ascii_whitespace does not include U+000B, and str::trim removes
/// the white space of Unicode, so the function gives the set.
fn is_space(c: char) -> bool {
    matches!(c, ' ' | '\t' | '\n' | '\x0b' | '\x0c' | '\r')
}

/// Returns s with the white space at the start and at the end removed.
pub fn trim(s: &str) -> &str {
    s.trim_matches(is_space)
}

/// Returns s with the white space at the start removed.
pub fn trim_start(s: &str) -> &str {
    s.trim_start_matches(is_space)
}
