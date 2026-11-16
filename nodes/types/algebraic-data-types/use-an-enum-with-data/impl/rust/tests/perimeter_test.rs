// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.

use algo_use_an_enum_with_data::{Shape, make_shape, perimeter};

#[test]
fn cases() {
    let cases: Vec<(&str, &str, Vec<i64>, Option<i64>)> = vec![
        ("square", "square", vec![3], Some(12)),
        ("rectangle", "rectangle", vec![2, 5], Some(14)),
        ("triangle", "triangle", vec![3, 4, 5], Some(12)),
        ("unknown-kind", "circle", vec![2], None),
        ("wrong-case", "Square", vec![3], None),
        ("too-few-sides", "rectangle", vec![4], None),
        ("too-many-sides", "square", vec![3, 3], None),
        ("no-sides", "triangle", vec![], None),
        ("zero-side", "square", vec![0], None),
        ("negative-side", "rectangle", vec![-2, 5], None),
        ("not-a-triangle", "triangle", vec![1, 2, 3], None),
        ("largest", "triangle", vec![1000000, 1000000, 1000000], Some(3000000)),
    ];
    for (name, kind, sides, want) in cases {
        assert_eq!(perimeter(kind, &sides), want, "case {name}");
    }
}

#[test]
fn variants_carry_their_data() {
    assert_eq!(
        make_shape("rectangle", &[2, 5]),
        Some(Shape::Rectangle { width: 2, height: 5 })
    );
}
