// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

/// Returns the code point index of the first match of sub in s, or -1.
pub fn find(s: &str, sub: &str) -> i64 {
    // str::find returns a byte index, so count the chars before it.
    s.find(sub).map_or(-1, |i| s[..i].chars().count() as i64)
}

/// Returns the code point index of the last match of sub in s, or -1.
pub fn find_last(s: &str, sub: &str) -> i64 {
    s.rfind(sub).map_or(-1, |i| s[..i].chars().count() as i64)
}
