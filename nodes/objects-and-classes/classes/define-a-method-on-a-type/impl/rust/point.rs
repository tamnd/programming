// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

/// A point on a grid. `Copy` makes it a plain value, like an integer.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Point {
    pub x: i32,
    pub y: i32,
}

pub const ORIGIN: Point = Point { x: 0, y: 0 };

impl Point {
    pub fn new(x: i32, y: i32) -> Self {
        Point { x, y }
    }

    /// Returns the point moved by (dx, dy). `self` is taken by value, which
    /// copies a `Copy` type, so the caller keeps its point.
    pub fn translate(self, dx: i32, dy: i32) -> Self {
        Point::new(self.x + dx, self.y + dy)
    }

    /// Returns the grid distance |dx| + |dy| to another point.
    pub fn manhattan(&self, other: &Point) -> i32 {
        (self.x - other.x).abs() + (self.y - other.y).abs()
    }
}

pub fn point_report(x: i32, y: i32, dx: i32, dy: i32) -> Vec<i32> {
    let p = Point::new(x, y);
    let q = p.translate(dx, dy);
    vec![q.x, q.y, p.manhattan(&q), q.manhattan(&ORIGIN)]
}
