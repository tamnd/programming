// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

use std::collections::HashMap;

/// Returns a line key=value for each entry in key order, then the total of the values.
pub fn score_lines(scores: &HashMap<&str, i32>) -> Vec<String> {
    // A HashMap gives its entries in arbitrary order, so sort them.
    let mut entries: Vec<(&&str, &i32)> = scores.iter().collect();
    entries.sort();
    let mut lines: Vec<String> = entries.iter().map(|(k, v)| format!("{k}={v}")).collect();
    let total: i32 = scores.values().sum();
    lines.push(format!("total={total}"));
    lines
}
