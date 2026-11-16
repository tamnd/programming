// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

/// Returns true if the ASCII letters and digits of text, in lower case, read
/// the same backwards. The text is ASCII, so the function works on bytes.
pub fn is_palindrome(text: &str) -> bool {
    let kept: Vec<u8> = text
        .bytes()
        .filter(u8::is_ascii_alphanumeric)
        .map(|b| b.to_ascii_lowercase())
        .collect();
    kept.iter().eq(kept.iter().rev())
}
