// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

//! Base64 with the two alphabets of RFC 4648 and a strict decoder. The
//! standard library has no Base64, so this file implements it.

use std::fmt;

/// `Standard` is RFC 4648 section 4, padded with `=`. `Url` is section 5,
/// with no padding.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Alphabet {
    Standard,
    Url,
}

/// The error for text that is not valid Base64.
#[derive(Debug, PartialEq, Eq)]
pub struct InvalidBase64;

impl fmt::Display for InvalidBase64 {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str("invalid base64 text")
    }
}

impl std::error::Error for InvalidBase64 {}

const STANDARD: &[u8; 64] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const URL: &[u8; 64] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
const NONE: u8 = 0xff;

/// Maps each byte to its value in the alphabet, or to `NONE`.
const fn values(chars: &[u8; 64]) -> [u8; 256] {
    let mut table = [NONE; 256];
    let mut i = 0;
    while i < 64 {
        table[chars[i] as usize] = i as u8;
        i += 1;
    }
    table
}

const STANDARD_VALUES: [u8; 256] = values(STANDARD);
const URL_VALUES: [u8; 256] = values(URL);

/// Returns the Base64 text of `data`.
pub fn encode(data: &[u8], alphabet: Alphabet) -> String {
    let chars = match alphabet {
        Alphabet::Standard => STANDARD,
        Alphabet::Url => URL,
    };
    let mut text = String::with_capacity(data.len().div_ceil(3) * 4);
    for chunk in data.chunks(3) {
        // Put the 1 to 3 bytes in the top 24 bits of n, with zero bits after.
        let n = chunk.iter().fold(0u32, |n, &b| (n << 8) | u32::from(b)) << (8 * (3 - chunk.len()));
        for i in 0..4 {
            if i <= chunk.len() {
                text.push(char::from(chars[((n >> (18 - 6 * i)) & 63) as usize]));
            } else if alphabet == Alphabet::Standard {
                text.push('=');
            }
        }
    }
    text
}

/// Returns the bytes of `text`. The text must be exactly what [`encode`]
/// gives for some bytes, so white space, a wrong padding and non-zero pad
/// bits are errors.
pub fn decode(text: &str, alphabet: Alphabet) -> Result<Vec<u8>, InvalidBase64> {
    let mut body = text.as_bytes();
    let values = match alphabet {
        Alphabet::Standard => {
            if body.len() % 4 != 0 {
                return Err(InvalidBase64);
            }
            // Remove at most two "=" at the end. Any other "=" is not in the
            // alphabet, so the loop below rejects it.
            for _ in 0..2 {
                if let [rest @ .., b'='] = body {
                    body = rest;
                }
            }
            &STANDARD_VALUES
        }
        Alphabet::Url => &URL_VALUES,
    };
    if body.len() % 4 == 1 {
        return Err(InvalidBase64);
    }
    let mut data = Vec::with_capacity(body.len() / 4 * 3 + 2);
    for chunk in body.chunks(4) {
        let mut n = 0u32;
        for &c in chunk {
            let v = values[usize::from(c)];
            if v == NONE {
                return Err(InvalidBase64);
            }
            n = (n << 6) | u32::from(v);
        }
        // 4, 3 or 2 characters hold 3, 2 or 1 bytes and 0, 2 or 4 pad bits.
        let bytes = chunk.len() * 6 / 8;
        let pad_bits = chunk.len() * 6 - bytes * 8;
        if n & ((1 << pad_bits) - 1) != 0 {
            return Err(InvalidBase64);
        }
        n >>= pad_bits;
        for i in (0..bytes).rev() {
            data.push((n >> (8 * i)) as u8);
        }
    }
    Ok(data)
}
