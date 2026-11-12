// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

/// Returns the UTF-8 bytes of `s`. A `str` is UTF-8, so `as_bytes` gives its
/// bytes, and `to_vec` copies them.
pub fn to_bytes(s: &str) -> Vec<u8> {
    s.as_bytes().to_vec()
}
