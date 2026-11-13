// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

/// Returns the words in order of the length, then of the word.
pub fn sort_by_length<'a>(words: &[&'a str]) -> Vec<&'a str> {
    let mut out = words.to_vec();
    // A tuple key compares the length first, then the word.
    out.sort_by_key(|w| (w.len(), *w));
    out
}
