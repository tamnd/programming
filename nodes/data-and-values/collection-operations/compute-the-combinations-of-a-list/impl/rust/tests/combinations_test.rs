// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

use algo_compute_the_combinations_of_a_list::combinations;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<i32>, usize, Vec<Vec<i32>>)> = vec![
        ("two-of-four", vec![1, 2, 3, 4], 2, vec![vec![1, 2], vec![1, 3], vec![1, 4], vec![2, 3], vec![2, 4], vec![3, 4]]),
        ("three-of-four", vec![1, 2, 3, 4], 3, vec![vec![1, 2, 3], vec![1, 2, 4], vec![1, 3, 4], vec![2, 3, 4]]),
        ("zero-k", vec![1, 2], 0, vec![Vec::new()]),
        ("empty", Vec::new(), 0, vec![Vec::new()]),
        ("k-too-large", vec![1, 2], 3, Vec::new()),
        ("all", vec![1, 2, 3], 3, vec![vec![1, 2, 3]]),
        ("duplicates", vec![1, 1, 2], 2, vec![vec![1, 1], vec![1, 2], vec![1, 2]]),
        ("unsorted", vec![3, 1, 2], 2, vec![vec![3, 1], vec![3, 2], vec![1, 2]]),
        ("extremes", vec![i32::MIN, i32::MAX], 1, vec![vec![i32::MIN], vec![i32::MAX]]),
    ];
    for (name, items, k, want) in cases {
        assert_eq!(combinations(&items, k), want, "{name}");
    }
}
