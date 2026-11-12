// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

/// Returns true if s has no bytes.
pub fn is_empty(s: &str) -> bool {
    s.is_empty()
}

/// Returns true if each character of s is ASCII white space. The code lists
/// the six characters, because char::is_ascii_whitespace does not accept
/// U+000B and str::trim also removes Unicode spaces.
pub fn is_blank(s: &str) -> bool {
    s.chars()
        .all(|c| matches!(c, ' ' | '\t' | '\n' | '\x0B' | '\x0C' | '\r'))
}
