// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

use algo_compute_the_permutations_of_a_list::permutations;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<i32>, Vec<Vec<i32>>)> = vec![
        ("three", vec![1, 2, 3], vec![vec![1, 2, 3], vec![1, 3, 2], vec![2, 1, 3], vec![2, 3, 1], vec![3, 1, 2], vec![3, 2, 1]]),
        ("two", vec![5, 6], vec![vec![5, 6], vec![6, 5]]),
        ("empty", Vec::new(), vec![Vec::new()]),
        ("single", vec![7], vec![vec![7]]),
        ("duplicates", vec![1, 1, 2], vec![vec![1, 1, 2], vec![1, 2, 1], vec![1, 1, 2], vec![1, 2, 1], vec![2, 1, 1], vec![2, 1, 1]]),
        ("unsorted", vec![3, 1, 2], vec![vec![3, 1, 2], vec![3, 2, 1], vec![1, 3, 2], vec![1, 2, 3], vec![2, 3, 1], vec![2, 1, 3]]),
        ("extremes", vec![i32::MIN, i32::MAX], vec![vec![i32::MIN, i32::MAX], vec![i32::MAX, i32::MIN]]),
    ];
    for (name, items, want) in cases {
        assert_eq!(permutations(&items), want, "{name}");
    }
}
