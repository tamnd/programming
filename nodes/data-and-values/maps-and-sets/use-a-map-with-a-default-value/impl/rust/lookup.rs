// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

use std::collections::HashMap;

/// Returns the value of each key in `table`, or `fallback` for a key not in `table`.
pub fn lookup(table: &HashMap<&str, i32>, keys: &[&str], fallback: i32) -> Vec<i32> {
    // get gives an Option, so a missing key and a stored 0 are different.
    keys.iter().map(|key| table.get(key).copied().unwrap_or(fallback)).collect()
}
