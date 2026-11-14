// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

//! Counts the lines, words and bytes of a UTF-8 file, as wc does.

use std::fs::File;
use std::io::{self, BufRead, BufReader, Read};
use std::path::Path;

/// The result: `lines` is the number of LF bytes, and a word is a run of one or
/// more chars that are not white space.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Default)]
pub struct Counts {
    pub lines: u64,
    pub words: u64,
    pub bytes: u64,
}

/// Counts UTF-8 text from `r`, one line at a time. An LF byte is never part of
/// a longer UTF-8 sequence, so each line decodes on its own. Bytes that are not
/// valid UTF-8 give an error of kind `InvalidData`.
pub fn count<R: Read>(r: R) -> io::Result<Counts> {
    let mut reader = BufReader::with_capacity(64 * 1024, r);
    let mut line = Vec::new();
    let mut c = Counts::default();
    loop {
        line.clear();
        // read_until retries on ErrorKind::Interrupted.
        let n = reader.read_until(b'\n', &mut line)?;
        if n == 0 {
            return Ok(c);
        }
        c.bytes += n as u64;
        let text = std::str::from_utf8(&line).map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?;
        let mut in_word = false;
        for ch in text.chars() {
            // char::is_whitespace is true for the chars that have the property White_Space.
            if ch.is_whitespace() {
                in_word = false;
                if ch == '\n' {
                    c.lines += 1;
                }
            } else if !in_word {
                in_word = true;
                c.words += 1;
            }
        }
    }
}

/// Counts the lines, words and bytes of the file at `path`.
pub fn word_count(path: impl AsRef<Path>) -> io::Result<Counts> {
    count(File::open(path)?)
}
