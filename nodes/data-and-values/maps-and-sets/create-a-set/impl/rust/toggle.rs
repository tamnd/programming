// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

use std::collections::HashSet;

/// Adds each word that is not in the set, and removes each word that is in it.
/// Returns the words of the set in ascending order.
pub fn toggle<'a>(words: &[&'a str]) -> Vec<&'a str> {
    let mut seen: HashSet<&'a str> = HashSet::new();
    for &word in words {
        // remove returns true if the set had the word.
        if !seen.remove(word) {
            seen.insert(word);
        }
    }
    let mut out: Vec<&'a str> = seen.into_iter().collect();
    out.sort_unstable();
    out
}
