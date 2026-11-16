// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

use algo_overload_an_operator::{vector_sum, Vec2};

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<i64>, Vec<i64>, [i64; 2])> = vec![
        ("two", vec![1, 3], vec![2, 4], [4, 6]),
        ("three", vec![1, 2, 3], vec![10, 20, 30], [6, 60]),
        ("empty", vec![], vec![], [0, 0]),
        ("single", vec![5], vec![-7], [5, -7]),
        ("negative", vec![-1, -2], vec![-3, -4], [-3, -7]),
        ("cancel", vec![3, -3], vec![5, -5], [0, 0]),
        ("limit", vec![1_000_000_000; 3], vec![-1_000_000_000; 3], [3_000_000_000, -3_000_000_000]),
    ];
    for (name, xs, ys, want) in cases {
        assert_eq!(vector_sum(&xs, &ys), Some(want), "case {name}");
    }
}

#[test]
fn operators() {
    let a = Vec2::new(1, 2);
    let b = Vec2::new(3, 4);
    assert_eq!(a + b, Vec2::new(4, 6));
    assert!(a != b && a == Vec2::new(1, 2));
    let mut c = a;
    c += b;
    assert_eq!(c, Vec2::new(4, 6));
    assert_eq!(vector_sum(&[1], &[]), None);
}
