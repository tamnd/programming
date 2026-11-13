// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

use algo_flatten_a_nested_list::flatten;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<Vec<i32>>, Vec<i32>)> = vec![
        ("three", vec![vec![1, 2], vec![3], vec![4, 5, 6]], vec![1, 2, 3, 4, 5, 6]),
        ("two", vec![vec![7], vec![8, 9]], vec![7, 8, 9]),
        ("empty", Vec::<Vec<i32>>::new(), Vec::<i32>::new()),
        ("empty-inner", vec![Vec::<i32>::new(), vec![1], Vec::<i32>::new()], vec![1]),
        ("all-empty", vec![Vec::<i32>::new(), Vec::<i32>::new()], Vec::<i32>::new()),
        ("single", vec![vec![5]], vec![5]),
        ("one-list", vec![vec![1, 2, 3]], vec![1, 2, 3]),
        ("negative", vec![vec![-1, -2], vec![3]], vec![-1, -2, 3]),
        ("extremes", vec![vec![i32::MIN], vec![2147483647, 0]], vec![i32::MIN, 2147483647, 0]),
    ];
    for (name, lists, want) in cases {
        assert_eq!(flatten(&lists), want, "{name}");
    }
}
