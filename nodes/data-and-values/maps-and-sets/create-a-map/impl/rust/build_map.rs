// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

use std::collections::HashMap;

/// Puts the pairs in a map, removes the keys in `remove`, and returns the entries sorted by key.
pub fn build_map<'a>(pairs: &[(&'a str, i32)], remove: &[&str]) -> Vec<(&'a str, i32)> {
    let mut map: HashMap<&'a str, i32> = HashMap::new();
    for &(key, value) in pairs {
        // insert adds the key, or replaces the value and returns the old one.
        map.insert(key, value);
    }
    for &key in remove {
        // remove returns None for a key that is not there.
        map.remove(key);
    }
    let mut out: Vec<(&'a str, i32)> = map.into_iter().collect();
    // A str compares by its bytes.
    out.sort();
    out
}
