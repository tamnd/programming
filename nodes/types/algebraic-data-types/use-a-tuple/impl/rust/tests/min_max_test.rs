// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

use algo_use_a_tuple::min_max;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<i32>, (i32, i32))> = vec![
        ("mixed", vec![3, 1, 2], (1, 3)),
        ("longer", vec![4, 9, -2, 7, 0], (-2, 9)),
        ("one", vec![5], (5, 5)),
        ("negative", vec![-3, -1, -7], (-7, -1)),
        ("all-same", vec![2, 2, 2], (2, 2)),
        ("int32-extremes", vec![i32::MAX, 0, i32::MIN], (i32::MIN, i32::MAX)),
        ("sorted", vec![1, 2, 3, 4], (1, 4)),
        ("reversed", vec![4, 3, 2, 1], (1, 4)),
    ];
    for (name, numbers, want) in cases {
        let (lo, hi) = min_max(&numbers); // destructuring
        assert_eq!((lo, hi), want, "case {name}");
    }
}
