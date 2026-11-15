// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

/// Joins the items at each index, up to the end of the shortest slice.
pub fn zip_join(lower: &[&str], upper: &[&str], digits: &[i32]) -> Vec<String> {
    let mut out = Vec::new();
    // Two zips make a pair inside a pair: ((a, b), d).
    for ((a, b), d) in lower.iter().zip(upper).zip(digits) {
        out.push(format!("{a}{b}{d}"));
    }
    out
}
