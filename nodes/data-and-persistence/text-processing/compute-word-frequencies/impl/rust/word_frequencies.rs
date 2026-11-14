// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

//! Finds the most frequent words of a text file.

use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::path::Path;

/// Adds 1 to the count of `word` and clears it, if it is not empty.
fn add(counts: &mut HashMap<Vec<u8>, u64>, word: &mut Vec<u8>) {
    if word.is_empty() {
        return;
    }
    match counts.get_mut(word.as_slice()) {
        Some(count) => *count += 1,
        None => {
            counts.insert(word.clone(), 1);
        }
    }
    word.clear();
}

/// Returns the `n` most frequent words of the file at `path`, with their
/// counts. A word is a run of ASCII letters, changed to lower case. The pairs
/// are sorted by count from high to low, then by word. A missing file gives an
/// error of the kind `ErrorKind::NotFound`.
pub fn word_frequencies(path: impl AsRef<Path>, n: usize) -> io::Result<Vec<(String, u64)>> {
    let mut reader = BufReader::with_capacity(64 * 1024, File::open(path)?);
    let mut counts: HashMap<Vec<u8>, u64> = HashMap::new();
    // The bytes of the current word. They stay across the reads of the buffer,
    // so a word that the end of a buffer cuts is not split.
    let mut word = Vec::new();
    loop {
        let buf = reader.fill_buf()?;
        if buf.is_empty() {
            break;
        }
        for &b in buf {
            if b.is_ascii_alphabetic() {
                word.push(b.to_ascii_lowercase());
            } else {
                add(&mut counts, &mut word);
            }
        }
        let len = buf.len();
        reader.consume(len);
    }
    add(&mut counts, &mut word);
    let mut ranked: Vec<(String, u64)> = counts
        .into_iter()
        .map(|(w, c)| (String::from_utf8(w).expect("a word holds only ASCII letters"), c))
        .collect();
    // No two entries have the same word, so an unstable sort gives one order.
    ranked.sort_unstable_by(|a, b| b.1.cmp(&a.1).then_with(|| a.0.cmp(&b.0)));
    ranked.truncate(n);
    Ok(ranked)
}
