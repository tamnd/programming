// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

use algo_declare_a_variable::total;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<i64>, i64)> = vec![
        ("one-two-three", vec![1, 2, 3], 6),
        ("four-values", vec![10, 20, 30, 40], 100),
        ("mixed-signs", vec![5, -2, 7], 10),
        ("empty", vec![], 0),
        ("single", vec![42], 42),
        ("all-negative", vec![-1, -2, -3], -6),
        ("zeros", vec![0, 0, 0], 0),
        ("cancel-out", vec![7, -7], 0),
        ("past-32-bits", vec![4000000000, 5000000000], 9000000000),
        ("eight-values", vec![1, 1, 1, 1, 1, 1, 1, 1], 8),
    ];
    for (name, values, want) in cases {
        assert_eq!(total(&values), want, "{name}");
    }
}
