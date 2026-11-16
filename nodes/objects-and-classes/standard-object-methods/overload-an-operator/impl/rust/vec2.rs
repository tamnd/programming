// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

//! A 2D vector with `+` from the `Add` trait and `==` from a derived `PartialEq`.

use std::iter::Sum;
use std::ops::{Add, AddAssign};

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, Hash)]
pub struct Vec2 {
    pub x: i64,
    pub y: i64,
}

impl Vec2 {
    pub fn new(x: i64, y: i64) -> Self {
        Vec2 { x, y }
    }
}

impl Add for Vec2 {
    type Output = Vec2;

    fn add(self, other: Vec2) -> Vec2 {
        Vec2::new(self.x + other.x, self.y + other.y)
    }
}

impl AddAssign for Vec2 {
    fn add_assign(&mut self, other: Vec2) {
        *self = *self + other;
    }
}

/// Lets `Iterator::sum` add vectors, starting from the zero vector.
impl Sum for Vec2 {
    fn sum<I: Iterator<Item = Vec2>>(iter: I) -> Vec2 {
        iter.fold(Vec2::default(), |a, b| a + b)
    }
}

/// Returns the sum of the vectors (xs[i], ys[i]) as [x, y], or None when the lengths differ.
pub fn vector_sum(xs: &[i64], ys: &[i64]) -> Option<[i64; 2]> {
    if xs.len() != ys.len() {
        return None;
    }
    let total: Vec2 = xs.iter().zip(ys).map(|(&x, &y)| Vec2::new(x, y)).sum();
    Some([total.x, total.y])
}
