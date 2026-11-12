// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

//! Concatenate two strings, and a string and a number.

/// Returns the characters of `a` followed by the characters of `b`.
pub fn concat(a: &str, b: &str) -> String {
    // String + &str takes the String on the left and appends to its buffer,
    // so the code makes an owned copy of a first.
    a.to_owned() + b
}

/// Returns `s` followed by the decimal form of `n`.
pub fn concat_number(s: &str, n: i32) -> String {
    format!("{s}{n}")
}
