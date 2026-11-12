// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

/// Returns s with a to z changed to A to Z. str::to_uppercase changes each
/// letter of Unicode, and to_ascii_uppercase changes only the ASCII letters.
pub fn to_upper(s: &str) -> String {
    s.to_ascii_uppercase()
}

/// Returns s with A to Z changed to a to z.
pub fn to_lower(s: &str) -> String {
    s.to_ascii_lowercase()
}
