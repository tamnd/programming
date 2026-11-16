// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

/// An enum whose variants carry different data.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Shape {
    Square { side: i64 },
    Rectangle { width: i64, height: i64 },
    Triangle { a: i64, b: i64, c: i64 },
}

/// Builds a shape, or returns None when the data is not a valid shape.
pub fn make_shape(kind: &str, sides: &[i64]) -> Option<Shape> {
    if sides.iter().any(|&s| s <= 0) {
        return None;
    }
    match (kind, sides) {
        ("square", &[side]) => Some(Shape::Square { side }),
        ("rectangle", &[width, height]) => Some(Shape::Rectangle { width, height }),
        ("triangle", &[a, b, c]) if a < b + c && b < a + c && c < a + b => {
            Some(Shape::Triangle { a, b, c })
        }
        _ => None,
    }
}

impl Shape {
    /// The match must cover every variant, or the program does not compile.
    pub fn perimeter(&self) -> i64 {
        match *self {
            Shape::Square { side } => 4 * side,
            Shape::Rectangle { width, height } => 2 * (width + height),
            Shape::Triangle { a, b, c } => a + b + c,
        }
    }
}

pub fn perimeter(kind: &str, sides: &[i64]) -> Option<i64> {
    make_shape(kind, sides).map(|s| s.perimeter())
}
