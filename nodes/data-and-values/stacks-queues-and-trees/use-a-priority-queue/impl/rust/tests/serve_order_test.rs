// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

use algo_use_a_priority_queue::serve_order;

#[test]
fn cases() {
    let (lo, hi) = (i32::MIN, i32::MAX);
    let cases: Vec<(&str, Vec<i32>, Vec<usize>)> = vec![
        ("example", vec![2, 5, 1, 5], vec![1, 3, 0, 2]),
        ("longer", vec![3, 1, 4, 1, 5, 9, 2, 6], vec![5, 7, 4, 2, 0, 6, 1, 3]),
        ("empty", vec![], vec![]),
        ("one", vec![7], vec![0]),
        ("ties", vec![4, 4, 4, 4], vec![0, 1, 2, 3]),
        ("negative", vec![-3, -1, -2], vec![1, 2, 0]),
        ("int32-extremes", vec![lo, hi, 0, lo], vec![1, 2, 0, 3]),
        ("sorted", vec![1, 2, 3], vec![2, 1, 0]),
        ("reversed", vec![3, 2, 1], vec![0, 1, 2]),
    ];
    for (name, priorities, want) in cases {
        assert_eq!(serve_order(&priorities), want, "case {name}");
    }
}
