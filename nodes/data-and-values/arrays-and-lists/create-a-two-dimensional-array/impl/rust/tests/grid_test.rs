// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

use algo_create_a_two_dimensional_array::grid;

#[test]
fn cases() {
    let want: Vec<Vec<i32>> = vec![vec![0, 0, 0], vec![0, 0, 0]];
    assert_eq!(grid(2, 3, 0), want, "two-by-three");
    let want: Vec<Vec<i32>> = vec![vec![7], vec![7], vec![7]];
    assert_eq!(grid(3, 1, 7), want, "three-by-one");
    let want: Vec<Vec<i32>> = vec![];
    assert_eq!(grid(0, 3, 1), want, "no-rows");
    let want: Vec<Vec<i32>> = vec![vec![], vec![]];
    assert_eq!(grid(2, 0, 1), want, "no-cols");
    let want: Vec<Vec<i32>> = vec![vec![5]];
    assert_eq!(grid(1, 1, 5), want, "one-cell");
    let want: Vec<Vec<i32>> = vec![vec![2, 2, 2, 2]];
    assert_eq!(grid(1, 4, 2), want, "one-row");
    let want: Vec<Vec<i32>> = vec![vec![2], vec![2], vec![2], vec![2]];
    assert_eq!(grid(4, 1, 2), want, "one-col");
    let want: Vec<Vec<i32>> = vec![vec![-3, -3], vec![-3, -3]];
    assert_eq!(grid(2, 2, -3), want, "negative");
    let want: Vec<Vec<i32>> = vec![vec![i32::MIN, i32::MIN], vec![i32::MIN, i32::MIN]];
    assert_eq!(grid(2, 2, i32::MIN), want, "min-int");
    let want: Vec<Vec<i32>> = vec![vec![2147483647, 2147483647]];
    assert_eq!(grid(1, 2, 2147483647), want, "max-int");
}

#[test]
fn rows_are_separate() {
    let mut g = grid(2, 3, 0);
    g[0][0] = 1;
    assert_eq!(g[1][0], 0);
}
