// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

use std::collections::HashMap;

/// A map key. The derived Hash and Eq use the same fields, so equal points
/// have equal hashes. The derived Ord gives the order of x, then y.
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub struct Point {
    pub x: i32,
    pub y: i32,
}

/// Returns (x, y, count) for each distinct point, in order of x, then y.
pub fn count_points(points: &[(i32, i32)]) -> Vec<(i32, i32, usize)> {
    let mut counts: HashMap<Point, usize> = HashMap::new();
    for &(x, y) in points {
        *counts.entry(Point { x, y }).or_insert(0) += 1;
    }
    let mut out: Vec<(Point, usize)> = counts.into_iter().collect();
    out.sort();
    out.into_iter().map(|(p, n)| (p.x, p.y, n)).collect()
}
