// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.

use algo_compare_floating_point_numbers::is_close;

#[test]
fn cases() {
    let cases: Vec<(&str, f64, f64, f64, f64, bool)> = vec![
        ("equal", 1.5, 1.5, 1e-9, 0.0, true),
        ("sum-of-tenths", 0.30000000000000004, 0.3, 1e-9, 0.0, true),
        ("different", 1.0, 1.1, 1e-9, 0.0, false),
        ("large-values", 10000000000.0, 10000000001.0, 1e-9, 0.0, true),
        ("zero-and-zero", 0.0, 0.0, 0.0, 0.0, true),
        ("one-ulp-apart", 1.0, 1.0000000000000002, 0.0, 0.0, false),
        ("near-zero-relative", 1e-12, 0.0, 1e-9, 0.0, false),
        ("near-zero-absolute", 1e-12, 0.0, 1e-9, 1e-9, true),
        ("opposite-signs", 1e-10, -1e-10, 1e-9, 0.0, false),
        ("opposite-signs-absolute", 1e-10, -1e-10, 1e-9, 1e-9, true),
        ("on-the-boundary", 10.0, 9.0, 0.1, 0.0, true),
        ("swapped-boundary", 9.0, 10.0, 0.1, 0.0, true),
        ("past-the-boundary", 10.0, 8.875, 0.1, 0.0, false),
        ("negative-values", -1.0, -1.0000000001, 1e-9, 0.0, true),
    ];
    for (name, a, b, rel_tol, abs_tol, want) in cases {
        assert_eq!(is_close(a, b, rel_tol, abs_tol), want, "{name}");
        assert_eq!(is_close(b, a, rel_tol, abs_tol), want, "{name} swapped");
    }
}

#[test]
fn non_finite() {
    assert!(!is_close(f64::NAN, f64::NAN, 1e-9, 0.0));
    assert!(is_close(f64::INFINITY, f64::INFINITY, 1e-9, 0.0));
    assert!(!is_close(f64::INFINITY, f64::NEG_INFINITY, 1e-9, 0.0));
    assert!(!is_close(f64::INFINITY, 1e308, 1e-9, 0.0));
}

#[test]
fn epsilon() {
    assert_eq!(f64::from_bits(1.0_f64.to_bits() + 1) - 1.0, f64::EPSILON);
}
