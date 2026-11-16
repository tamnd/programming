// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.

use algo_round_a_number::round_half_away;

#[test]
fn cases() {
    let cases: Vec<(&str, f64, i64)> = vec![
        ("down", 2.4, 2),
        ("up", 2.6, 3),
        ("negative", -2.6, -3),
        ("integer", 7.0, 7),
        ("half-even-up", 2.5, 3),
        ("half-odd-up", 1.5, 2),
        ("half-zero", 0.5, 1),
        ("half-negative", -2.5, -3),
        ("half-negative-small", -0.5, -1),
        ("negative-below-half", -0.4, 0),
        ("near-half", 0.49999999999999994, 0),
        ("zero", 0.0, 0),
        ("large-odd", 4503599627370497.0, 4503599627370497),
        ("large-half-negative", -1000000000000000.5, -1000000000000001),
    ];
    for (name, x, want) in cases {
        assert_eq!(round_half_away(x), want, "{name}");
    }
}

#[test]
fn library_modes() {
    assert_eq!(2.5_f64.round_ties_even(), 2.0);
    assert_eq!((-2.5_f64).floor(), -3.0);
    assert_eq!((-2.5_f64).trunc(), -2.0);
    assert_eq!((-2.5_f64).ceil(), -2.0);
}
