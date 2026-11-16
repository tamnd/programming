// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

use algo_use_mathematical_constants_and_functions::to_cartesian;
use std::f64::consts::PI;

const CASES: &[(&str, f64, f64, (f64, f64))] = &[
    ("east", 1.0, 0.0, (1.0, 0.0)),
    ("north", 2.0, 90.0, (0.0, 2.0)),
    ("west", 2.0, 180.0, (-2.0, 0.0)),
    ("diagonal", 1.0, 45.0, (0.7071067811865476, 0.7071067811865476)),
    ("thirty-degrees", 2.0, 30.0, (1.7320508075688772, 1.0)),
    ("zero-radius", 0.0, 123.0, (0.0, 0.0)),
    ("south", 1.0, 270.0, (0.0, -1.0)),
    ("negative-angle", 1.0, -90.0, (0.0, -1.0)),
    ("full-turn", 1.0, 360.0, (1.0, 0.0)),
    ("two-turns", 3.0, 720.0, (3.0, 0.0)),
    ("negative-radius", -1.0, 0.0, (-1.0, 0.0)),
    ("large-radius", 1000000.0, 60.0, (500000.0, 866025.4037844386)),
];

fn close(got: f64, want: f64) -> bool {
    (got - want).abs() <= f64::max(1e-9, 1e-12 * want.abs())
}

#[test]
fn cases() {
    for &(name, r, degrees, (x, y)) in CASES {
        let (gx, gy) = to_cartesian(r, degrees);
        assert!(close(gx, x) && close(gy, y), "{name}: got ({gx}, {gy})");
    }
}

#[test]
fn constants() {
    assert!(close(0f64.atan2(-1.0), PI));
    assert_eq!(16f64.sqrt(), 4.0);
    assert_eq!(2f64.powi(10), 1024.0);
}
