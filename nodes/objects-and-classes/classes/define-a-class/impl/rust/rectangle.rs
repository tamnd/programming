// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

use std::fmt;

/// A rectangle with a width and a height. Rust has no classes: a struct holds
/// the fields, and an impl block holds the methods.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle {
    /// An associated function with the conventional name new. Rust has no
    /// constructors.
    pub fn new(width: u32, height: u32) -> Self {
        Self { width, height }
    }

    /// &self borrows the rectangle to read it.
    pub fn area(&self) -> u32 {
        self.width * self.height
    }

    pub fn perimeter(&self) -> u32 {
        2 * (self.width + self.height)
    }

    /// &mut self borrows the rectangle to change it.
    pub fn scale(&mut self, factor: u32) {
        self.width *= factor;
        self.height *= factor;
    }
}

impl fmt::Display for Rectangle {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Rectangle({}, {})", self.width, self.height)
    }
}

/// Returns the area and the perimeter, before and after scale(factor).
pub fn rectangle_report(width: u32, height: u32, factor: u32) -> Vec<u32> {
    let mut r = Rectangle::new(width, height);
    let mut out = vec![r.area(), r.perimeter()];
    r.scale(factor);
    out.extend([r.area(), r.perimeter()]);
    out
}
