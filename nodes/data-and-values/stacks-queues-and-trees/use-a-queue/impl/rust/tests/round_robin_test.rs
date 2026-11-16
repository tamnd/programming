// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

use algo_use_a_queue::finish_order;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<u32>, u32, Vec<usize>)> = vec![
        ("example", vec![3, 1, 2], 1, vec![1, 2, 0]),
        ("slice-two", vec![4, 1, 3, 2], 2, vec![1, 3, 0, 2]),
        ("empty", vec![], 3, vec![]),
        ("one", vec![7], 2, vec![0]),
        ("big-slice", vec![3, 1, 2], 10, vec![0, 1, 2]),
        ("slice-one", vec![2, 2, 1], 1, vec![2, 0, 1]),
        ("equal-work", vec![2, 2, 2], 1, vec![0, 1, 2]),
        ("long-job-first", vec![10, 1, 1], 3, vec![1, 2, 0]),
    ];
    for (name, work, slice, want) in cases {
        assert_eq!(finish_order(&work, slice), want, "case {name}");
    }
}
