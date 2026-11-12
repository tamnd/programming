// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

//! Find the lengths of a string in UTF-8 bytes, in UTF-16 code units and in
//! code points.

/// Returns the length of `s` in UTF-8 bytes, in UTF-16 code units and in
/// code points.
pub fn lengths(s: &str) -> [usize; 3] {
    // A str is UTF-8, so len counts bytes. chars yields code points.
    [s.len(), s.encode_utf16().count(), s.chars().count()]
}
