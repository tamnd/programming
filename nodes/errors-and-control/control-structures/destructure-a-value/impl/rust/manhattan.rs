// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

/// Returns the Manhattan distance between the points p = [x, y] and q = [x, y].
pub fn manhattan(p: [i32; 2], q: [i32; 2]) -> i32 {
    // An array of fixed size always matches [x, y], so let can take it apart.
    let [x1, y1] = p;
    let [x2, y2] = q;
    (x1 - x2).abs() + (y1 - y2).abs()
}
