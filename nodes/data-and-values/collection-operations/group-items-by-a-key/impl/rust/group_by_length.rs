// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

use std::collections::BTreeMap;

/// Returns one (length, words) pair for each length, in ascending order of the length.
pub fn group_by_length<'a>(words: &[&'a str]) -> Vec<(usize, Vec<&'a str>)> {
    // A BTreeMap gives its keys in ascending order.
    let mut groups: BTreeMap<usize, Vec<&'a str>> = BTreeMap::new();
    for &w in words {
        groups.entry(w.len()).or_default().push(w);
    }
    groups.into_iter().collect()
}
