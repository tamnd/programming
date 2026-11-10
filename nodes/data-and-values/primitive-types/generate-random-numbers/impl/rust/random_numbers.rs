// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

//! Random integers in a range, a shuffle, a seeded generator and tokens.
//!
//! The standard library has no random number generator, so this module has
//! `SplitMix64` for seeded words, and it reads `/dev/urandom` for secure bytes,
//! which works on Linux and macOS.

use std::fmt::Write as _;
use std::fs::File;
use std::io::{self, Read};

const WORDS: u64 = 1 << 32;

/// Returns an integer from `lo` to `hi`, each with equal probability.
///
/// `next_word` returns a random integer from 0 to 2^32 - 1. A word below
/// 2^32 % n is rejected, so that each remainder comes from the same number
/// of words.
///
/// # Panics
///
/// Panics if `hi < lo` or if the range has more than 2^32 values.
pub fn int_in_range(next_word: &mut impl FnMut() -> u32, lo: i64, hi: i64) -> i64 {
    let span = hi.abs_diff(lo);
    assert!(
        lo <= hi && span < WORDS,
        "need lo <= hi and at most 2^32 values"
    );
    let n = span + 1;
    let least = WORDS % n;
    loop {
        let word = u64::from(next_word());
        if word >= least {
            // word % n <= hi - lo, so the sum is at most hi.
            return lo.wrapping_add_unsigned(word % n);
        }
    }
}

/// Shuffles `items` in place with the Fisher-Yates method.
///
/// # Panics
///
/// Panics if `items` has more than 2^32 items.
pub fn shuffle<T>(next_word: &mut impl FnMut() -> u32, items: &mut [T]) {
    for i in (1..items.len()).rev() {
        let top = i64::try_from(i).expect("a slice index fits in i64");
        let j = usize::try_from(int_in_range(next_word, 0, top)).expect("0 <= j <= i");
        items.swap(i, j);
    }
}

/// Returns a word source that gives the same words for the same seed.
///
/// This is `SplitMix64` (Steele, Lea and Flood), as in the reference code of
/// Sebastiano Vigna, with the top 32 bits of each output.
pub fn seeded_words(seed: u64) -> impl FnMut() -> u32 {
    let mut state = seed;
    move || {
        state = state.wrapping_add(0x9e37_79b9_7f4a_7c15);
        let mut z = state;
        z = (z ^ (z >> 30)).wrapping_mul(0xbf58_476d_1ce4_e5b9);
        z = (z ^ (z >> 27)).wrapping_mul(0x94d0_49bb_1331_11eb);
        z ^= z >> 31;
        (z >> 32) as u32
    }
}

/// Fills `buf` with secure random bytes from `/dev/urandom`.
///
/// # Errors
///
/// Returns the error of the open or the read.
pub fn secure_bytes(buf: &mut [u8]) -> io::Result<()> {
    File::open("/dev/urandom")?.read_exact(buf)
}

/// Returns a word source from `/dev/urandom`.
///
/// # Errors
///
/// Returns the error if the file does not open.
///
/// # Panics
///
/// The source panics if a read fails.
pub fn secure_words() -> io::Result<impl FnMut() -> u32> {
    let mut file = File::open("/dev/urandom")?;
    Ok(move || {
        let mut b = [0u8; 4];
        file.read_exact(&mut b).expect("read /dev/urandom");
        u32::from_le_bytes(b)
    })
}

/// Returns `nbytes` secure random bytes as 2 * `nbytes` lowercase
/// hexadecimal digits.
///
/// # Errors
///
/// Returns the error of [`secure_bytes`].
pub fn token_hex(nbytes: usize) -> io::Result<String> {
    let mut bytes = vec![0u8; nbytes];
    secure_bytes(&mut bytes)?;
    let mut out = String::with_capacity(2 * nbytes);
    for b in bytes {
        write!(out, "{b:02x}").expect("a write to a String does not fail");
    }
    Ok(out)
}
