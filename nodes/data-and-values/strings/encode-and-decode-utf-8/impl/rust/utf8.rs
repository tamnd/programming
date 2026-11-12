// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

/// Returns the UTF-8 bytes of the code point `cp`.
///
/// # Panics
///
/// Panics if `cp` is a surrogate or above 0x10FFFF.
pub fn encode_utf8(cp: u32) -> Vec<u8> {
    let c = char::from_u32(cp).expect("a Unicode scalar value");
    let mut buf = [0u8; 4];
    c.encode_utf8(&mut buf).as_bytes().to_vec()
}

/// Returns the code point of the one UTF-8 sequence in `b`, or -1.
/// `std::str::from_utf8` returns an error for each sequence that is not well
/// formed.
pub fn decode_utf8(b: &[u8]) -> i64 {
    let Ok(s) = std::str::from_utf8(b) else {
        return -1;
    };
    let mut chars = s.chars();
    match (chars.next(), chars.next()) {
        (Some(c), None) => i64::from(u32::from(c)),
        _ => -1,
    }
}
