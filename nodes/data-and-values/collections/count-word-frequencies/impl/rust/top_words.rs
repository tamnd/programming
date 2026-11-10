// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

//! Counts the words of a text and returns the most frequent ones.

use std::collections::HashMap;

/// Tells if `c` ends a word: an ASCII character that is not a letter or a digit.
fn is_separator(c: char) -> bool {
    c.is_ascii() && !c.is_ascii_alphanumeric()
}

/// Returns the `n` most frequent words of `text` with their counts, by count
/// from high to low, then by word in code point order.
pub fn top_words(text: &str, n: usize) -> Vec<(String, u64)> {
    let mut counts: HashMap<String, u64> = HashMap::new();
    for word in text.split(is_separator).filter(|w| !w.is_empty()) {
        *counts.entry(word.to_lowercase()).or_insert(0) += 1;
    }
    let mut out: Vec<(String, u64)> = counts.into_iter().collect();
    // The order of a HashMap is not specified, so sort by both keys. A String
    // compares by bytes, which for UTF-8 is code point order.
    out.sort_unstable_by(|a, b| b.1.cmp(&a.1).then_with(|| a.0.cmp(&b.0)));
    out.truncate(n);
    out
}
