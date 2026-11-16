// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

/// Returns the point (x, y) at the distance r and the angle in degrees.
pub fn to_cartesian(r: f64, degrees: f64) -> (f64, f64) {
    let t = degrees.to_radians();
    (r * t.cos(), r * t.sin())
}
