// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

use algo_detect_division_by_zero::safe_div;

const MAX: i64 = i64::MAX;
const MIN: i64 = i64::MIN;

#[test]
fn cases() {
    let cases: Vec<(&str, i64, i64, Option<i64>)> = vec![
        ("exact", 12, 4, Some(3)),
        ("remainder-dropped", 7, 2, Some(3)),
        ("zero-dividend", 0, 5, Some(0)),
        ("zero-divisor", 5, 0, None),
        ("negative-by-zero", -5, 0, None),
        ("zero-by-zero", 0, 0, None),
        ("negative-dividend", -7, 2, Some(-3)),
        ("negative-divisor", 7, -2, Some(-3)),
        ("both-negative", -7, -2, Some(3)),
        ("min-by-minus-one", MIN, -1, None),
        ("min-by-one", MIN, 1, Some(MIN)),
        ("max-by-minus-one", MAX, -1, Some(-MAX)),
        ("min-by-max", MIN, MAX, Some(-1)),
    ];
    for (name, a, b, want) in cases {
        assert_eq!(safe_div(a, b), want, "case {name}");
    }
}

#[test]
fn float_division_by_zero() {
    let zero = 0.0_f64;
    assert_eq!(1.0 / zero, f64::INFINITY);
    assert!((zero / zero).is_nan());
}
