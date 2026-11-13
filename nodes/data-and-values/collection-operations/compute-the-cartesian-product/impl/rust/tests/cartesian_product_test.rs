// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

use algo_compute_the_cartesian_product::cartesian_product;

#[test]
fn cases() {
    let none: Vec<Vec<i32>> = Vec::new();
    let cases: Vec<(&str, Vec<Vec<i32>>, Vec<Vec<i32>>)> = vec![
        ("two", vec![vec![1, 2], vec![3, 4]], vec![vec![1, 3], vec![1, 4], vec![2, 3], vec![2, 4]]),
        ("three", vec![vec![1, 2], vec![3], vec![4, 5]], vec![vec![1, 3, 4], vec![1, 3, 5], vec![2, 3, 4], vec![2, 3, 5]]),
        ("no-lists", none.clone(), vec![Vec::new()]),
        ("one-empty", vec![vec![1, 2], Vec::new()], none.clone()),
        ("single", vec![vec![7, 8, 9]], vec![vec![7], vec![8], vec![9]]),
        ("duplicates", vec![vec![1, 1], vec![2]], vec![vec![1, 2], vec![1, 2]]),
        ("negative", vec![vec![-1], vec![-2, -3]], vec![vec![-1, -2], vec![-1, -3]]),
        ("extremes", vec![vec![i32::MIN, i32::MAX], vec![0]], vec![vec![i32::MIN, 0], vec![i32::MAX, 0]]),
    ];
    for (name, lists, want) in cases {
        assert_eq!(cartesian_product(&lists), want, "{name}");
    }
}
