// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

use algo_pattern_match_on_a_value::classify;

#[test]
fn cases() {
    let cases: &[(&str, i32, i32, &str)] = &[
        ("x-axis", 5, 0, "x-axis"),
        ("y-axis", 0, -3, "y-axis"),
        ("diagonal", 4, 4, "diagonal"),
        ("other", 2, 7, "other"),
        ("origin", 0, 0, "origin"),
        ("negative-diagonal", -2, -2, "diagonal"),
        ("anti-diagonal", 3, -3, "other"),
        ("negative-x-axis", -7, 0, "x-axis"),
        ("one-one", 1, 1, "diagonal"),
    ];
    for &(name, x, y, want) in cases {
        assert_eq!(classify(x, y), want, "{name}");
    }
}
