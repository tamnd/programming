// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

/// Returns the counts of the letters a to z in text, in the order of the
/// alphabet. Upper case letters count as lower case letters. The text is
/// ASCII, so the function reads bytes.
pub fn count_letters(text: &str) -> Vec<i64> {
    let mut counts = vec![0; 26];
    for b in text.bytes().filter(u8::is_ascii_alphabetic) {
        counts[usize::from(b.to_ascii_lowercase() - b'a')] += 1;
    }
    counts
}
