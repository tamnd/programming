// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

/// Returns the code points of s from index start up to, not including, end.
pub fn substring(s: &str, start: usize, end: usize) -> String {
    // &s[i..j] takes byte indexes and panics inside a character, so the code
    // finds the byte index of each code point index.
    s[offset(s, start)..offset(s, end)].to_string()
}

// offset returns the byte index of code point k, or s.len() for the end.
fn offset(s: &str, k: usize) -> usize {
    s.char_indices().nth(k).map_or(s.len(), |(i, _)| i)
}
