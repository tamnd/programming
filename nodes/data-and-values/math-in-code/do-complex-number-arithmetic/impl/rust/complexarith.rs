// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

use std::ops::{Add, Div, Mul};

/// A complex number. The standard library has no complex type; the crate
/// num-complex has a complete one.
#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Complex {
    pub re: f64,
    pub im: f64,
}

impl Add for Complex {
    type Output = Complex;
    fn add(self, w: Complex) -> Complex {
        Complex { re: self.re + w.re, im: self.im + w.im }
    }
}

impl Mul for Complex {
    type Output = Complex;
    fn mul(self, w: Complex) -> Complex {
        Complex { re: self.re * w.re - self.im * w.im, im: self.re * w.im + self.im * w.re }
    }
}

impl Div for Complex {
    type Output = Complex;
    /// Divides by the larger part of w first, so the formula cannot overflow.
    fn div(self, w: Complex) -> Complex {
        let (a, b, c, d) = (self.re, self.im, w.re, w.im);
        if c.abs() >= d.abs() {
            let r = d / c;
            let t = c + d * r;
            Complex { re: (a + b * r) / t, im: (b - a * r) / t }
        } else {
            let r = c / d;
            let t = c * r + d;
            Complex { re: (a * r + b) / t, im: (b * r - a) / t }
        }
    }
}

impl Complex {
    pub fn abs(self) -> f64 {
        self.re.hypot(self.im)
    }

    pub fn phase(self) -> f64 {
        self.im.atan2(self.re)
    }
}

/// Returns the sum, the product and the quotient of a and b, then abs(a) and
/// phase(a), as 8 numbers. a and b are pairs [re, im].
pub fn complex_arithmetic(a: &[f64], b: &[f64]) -> [f64; 8] {
    let z = Complex { re: a[0], im: a[1] };
    let w = Complex { re: b[0], im: b[1] };
    let (s, p, q) = (z + w, z * w, z / w);
    [s.re, s.im, p.re, p.im, q.re, q.im, z.abs(), z.phase()]
}
