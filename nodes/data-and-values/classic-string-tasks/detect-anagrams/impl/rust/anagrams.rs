// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

/// Returns the count of each ASCII letter of text, with upper case as lower case.
fn counts(text: &str) -> [usize; 26] {
    let mut c = [0; 26];
    for b in text.bytes().filter(u8::is_ascii_alphabetic) {
        c[usize::from(b.to_ascii_lowercase() - b'a')] += 1;
    }
    c
}

/// Returns true if a and b hold each letter from a to z the same number of
/// times. Case, spaces, punctuation and digits do not count.
pub fn are_anagrams(a: &str, b: &str) -> bool {
    counts(a) == counts(b)
}
