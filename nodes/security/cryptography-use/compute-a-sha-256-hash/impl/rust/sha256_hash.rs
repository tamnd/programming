// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

//! SHA-256 as FIPS 180-4 defines it. The Rust standard library has no
//! SHA-256, so this file holds a short implementation.

use std::fs::File;
use std::io::{self, Read};
use std::path::Path;

// FIPS 180-4, 4.2.2: the first 32 bits of the fractional parts of the cube
// roots of the first 64 primes.
const K: [u32; 64] = [
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
];

// FIPS 180-4, 5.3.3: the initial hash value.
const H0: [u32; 8] = [
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
];

/// A SHA-256 computation that takes the message in parts.
#[derive(Clone)]
pub struct Sha256 {
    state: [u32; 8],
    block: [u8; 64],
    used: usize,
    // The length of the message in bytes. FIPS 180-4 allows fewer than 2^64 bits.
    len: u64,
}

impl Default for Sha256 {
    fn default() -> Self {
        Self::new()
    }
}

impl Sha256 {
    pub fn new() -> Sha256 {
        Sha256 {
            state: H0,
            block: [0; 64],
            used: 0,
            len: 0,
        }
    }

    /// Adds the bytes of data to the message.
    pub fn update(&mut self, mut data: &[u8]) {
        self.len = self.len.wrapping_add(data.len() as u64);
        while !data.is_empty() {
            let n = (64 - self.used).min(data.len());
            self.block[self.used..self.used + n].copy_from_slice(&data[..n]);
            self.used += n;
            data = &data[n..];
            if self.used == 64 {
                compress(&mut self.state, &self.block);
                self.used = 0;
            }
        }
    }

    /// Pads the message and returns the 32-byte digest.
    pub fn finalize(mut self) -> [u8; 32] {
        // FIPS 180-4, 5.1.1: a 1 bit, then zero bits, then the length in bits
        // as a 64-bit big-endian number, to a multiple of 512 bits.
        let bits = self.len.wrapping_mul(8);
        self.block[self.used] = 0x80;
        self.block[self.used + 1..].fill(0);
        if self.used >= 56 {
            // No room for the length: it goes in one more block.
            compress(&mut self.state, &self.block);
            self.block = [0; 64];
        }
        self.block[56..].copy_from_slice(&bits.to_be_bytes());
        compress(&mut self.state, &self.block);
        let mut out = [0u8; 32];
        for (chunk, word) in out.chunks_exact_mut(4).zip(self.state) {
            chunk.copy_from_slice(&word.to_be_bytes());
        }
        out
    }
}

// FIPS 180-4, 6.2.2: the hash computation for one 512-bit block.
fn compress(state: &mut [u32; 8], block: &[u8; 64]) {
    let mut w = [0u32; 64];
    for (t, word) in block.chunks_exact(4).enumerate() {
        w[t] = u32::from_be_bytes([word[0], word[1], word[2], word[3]]);
    }
    for t in 16..64 {
        let s0 = w[t - 15].rotate_right(7) ^ w[t - 15].rotate_right(18) ^ (w[t - 15] >> 3);
        let s1 = w[t - 2].rotate_right(17) ^ w[t - 2].rotate_right(19) ^ (w[t - 2] >> 10);
        w[t] = s1
            .wrapping_add(w[t - 7])
            .wrapping_add(s0)
            .wrapping_add(w[t - 16]);
    }
    let [mut a, mut b, mut c, mut d, mut e, mut f, mut g, mut h] = *state;
    for t in 0..64 {
        let big_s1 = e.rotate_right(6) ^ e.rotate_right(11) ^ e.rotate_right(25);
        let ch = (e & f) ^ (!e & g);
        let t1 = h
            .wrapping_add(big_s1)
            .wrapping_add(ch)
            .wrapping_add(K[t])
            .wrapping_add(w[t]);
        let big_s0 = a.rotate_right(2) ^ a.rotate_right(13) ^ a.rotate_right(22);
        let maj = (a & b) ^ (a & c) ^ (b & c);
        let t2 = big_s0.wrapping_add(maj);
        h = g;
        g = f;
        f = e;
        e = d.wrapping_add(t1);
        d = c;
        c = b;
        b = a;
        a = t1.wrapping_add(t2);
    }
    for (s, v) in state.iter_mut().zip([a, b, c, d, e, f, g, h]) {
        *s = s.wrapping_add(v);
    }
}

/// Returns the digest as 64 lowercase hexadecimal digits.
pub fn to_hex(digest: &[u8; 32]) -> String {
    // {:02x} keeps the leading zero of each byte.
    digest.iter().map(|b| format!("{b:02x}")).collect()
}

/// Returns the SHA-256 of the bytes of data, as lowercase hex.
pub fn sha256_hex(data: &[u8]) -> String {
    let mut h = Sha256::new();
    h.update(data);
    to_hex(&h.finalize())
}

/// Returns the SHA-256 of the UTF-8 bytes of text, as lowercase hex.
/// A Rust str is always UTF-8, so its bytes are the encoding.
pub fn sha256_text(text: &str) -> String {
    sha256_hex(text.as_bytes())
}

/// Returns the SHA-256 of the bytes of the file at path, as lowercase hex.
/// The file is read in chunks, so a large file does not fill the memory.
pub fn sha256_file(path: impl AsRef<Path>) -> io::Result<String> {
    let mut file = File::open(path)?;
    let mut h = Sha256::new();
    let mut buf = [0u8; 8192];
    loop {
        match file.read(&mut buf) {
            Ok(0) => break,
            Ok(n) => h.update(&buf[..n]),
            Err(e) if e.kind() == io::ErrorKind::Interrupted => continue,
            Err(e) => return Err(e),
        }
    }
    Ok(to_hex(&h.finalize()))
}
