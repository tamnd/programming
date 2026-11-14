// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

use std::collections::HashMap;

/// Returns the entries of both maps in key order; the value of `over` wins.
pub fn merge_maps<'a>(base: &HashMap<&'a str, i32>, over: &HashMap<&'a str, i32>) -> Vec<(&'a str, i32)> {
    // The clone keeps base as it is. extend replaces the values of keys that are there.
    let mut merged = base.clone();
    merged.extend(over.iter().map(|(&k, &v)| (k, v)));
    let mut out: Vec<(&'a str, i32)> = merged.into_iter().collect();
    out.sort();
    out
}
