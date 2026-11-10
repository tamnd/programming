// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

//! Split a string on a literal separator and on white space. The standard
//! library has no regular expressions, so there is no `split_regex` here.

/// Splits `s` on the literal separator `sep` and keeps the empty fields.
/// With `limit` n > 0 the result has at most n fields, and the last field
/// holds the rest of `s`. A limit of 0 means no limit.
pub fn split<'a>(s: &'a str, sep: &str, limit: usize) -> Vec<&'a str> {
    if limit == 0 {
        s.split(sep).collect()
    } else {
        s.splitn(limit, sep).collect()
    }
}

/// Splits `s` on runs of ASCII white space. The result has no empty fields.
pub fn split_whitespace(s: &str) -> Vec<&str> {
    s.split_ascii_whitespace().collect()
}
