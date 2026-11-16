// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

use algo_swap_two_values::swap_items;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<i64>, usize, usize, Vec<i64>)> = vec![
        ("first-and-last", vec![1, 2, 3], 0, 2, vec![3, 2, 1]),
        ("middle-pair", vec![10, 20, 30, 40], 1, 2, vec![10, 30, 20, 40]),
        ("two-items", vec![5, 7], 0, 1, vec![7, 5]),
        ("single-item", vec![9], 0, 0, vec![9]),
        ("same-index", vec![1, 2, 3], 1, 1, vec![1, 2, 3]),
        ("higher-index-first", vec![1, 2, 3, 4], 3, 0, vec![4, 2, 3, 1]),
        ("equal-values", vec![4, 4], 0, 1, vec![4, 4]),
        ("negative-values", vec![-1, 0, 1], 0, 2, vec![1, 0, -1]),
        ("int32-extremes", vec![2147483647, -2147483648], 0, 1, vec![-2147483648, 2147483647]),
        ("eight-items", vec![1, 2, 3, 4, 5, 6, 7, 8], 2, 6, vec![1, 2, 7, 4, 5, 6, 3, 8]),
    ];
    for (name, xs, i, j, want) in cases {
        assert_eq!(swap_items(&xs, i, j), want, "{name}");
    }
}

#[test]
fn mem_swap() {
    let mut a = String::from("left");
    let mut b = String::from("right");
    std::mem::swap(&mut a, &mut b);
    assert_eq!((a.as_str(), b.as_str()), ("right", "left"));
}
