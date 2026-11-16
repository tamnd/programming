// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

/// Names the place of the point (x, y): origin, x-axis, y-axis, diagonal or other.
pub fn classify(x: i32, y: i32) -> &'static str {
    match (x, y) {
        (0, 0) => "origin",
        (_, 0) => "x-axis",
        (0, _) => "y-axis",
        (a, b) if a == b => "diagonal",
        _ => "other",
    }
}
