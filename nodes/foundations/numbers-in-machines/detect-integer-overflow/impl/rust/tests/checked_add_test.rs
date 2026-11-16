// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

use algo_detect_integer_overflow::checked_add;

const MAX: i64 = i64::MAX;
const MIN: i64 = i64::MIN;

#[test]
fn cases() {
    let cases: Vec<(&str, i64, i64, Option<i64>)> = vec![
        ("small", 2, 3, Some(5)),
        ("negative", -7, 4, Some(-3)),
        ("beyond-32-bit", 4000000000, 5000000000, Some(9000000000)),
        ("max-plus-zero", MAX, 0, Some(MAX)),
        ("max-plus-one", MAX, 1, None),
        ("max-plus-max", MAX, MAX, None),
        ("min-plus-zero", MIN, 0, Some(MIN)),
        ("min-minus-one", MIN, -1, None),
        ("min-plus-min", MIN, MIN, None),
        ("min-plus-max", MIN, MAX, Some(-1)),
        ("max-minus-one", MAX, -1, Some(MAX - 1)),
    ];
    for (name, a, b, want) in cases {
        assert_eq!(checked_add(a, b), want, "case {name}");
    }
}

#[test]
fn other_overflow_methods() {
    assert_eq!(MAX.wrapping_add(1), MIN);
    assert_eq!(MAX.overflowing_add(1), (MIN, true));
    assert_eq!(MAX.saturating_add(1), MAX);
}
