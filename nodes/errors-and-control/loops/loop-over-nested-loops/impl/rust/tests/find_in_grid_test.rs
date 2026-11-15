// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

use algo_loop_over_nested_loops::find_in_grid;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<Vec<i32>>, i32, (isize, isize))> = vec![
        ("middle", vec![vec![1, 2, 3], vec![4, 5, 6], vec![7, 8, 9]], 5, (1, 1)),
        ("first-twenty", vec![vec![3, 8, 12], vec![5, 20, 20], vec![20, 1, 4]], 20, (1, 1)),
        ("first-cell", vec![vec![9, 1], vec![2, 3]], 9, (0, 0)),
        ("last-cell", vec![vec![1, 2], vec![3, 9]], 9, (1, 1)),
        ("row-major", vec![vec![1, 2, 9], vec![9, 3, 4]], 9, (0, 2)),
        ("missing", vec![vec![1, 2], vec![3, 4]], 9, (-1, -1)),
        ("empty-grid", vec![], 1, (-1, -1)),
        ("empty-rows", vec![vec![], vec![], vec![5]], 5, (2, 0)),
        ("ragged", vec![vec![1], vec![2, 3, 4], vec![5, 6]], 6, (2, 1)),
        ("negative", vec![vec![-1, -2], vec![-3, -4]], -3, (1, 0)),
    ];
    for (name, grid, target, want) in cases {
        assert_eq!(find_in_grid(&grid, target), want, "{name}");
    }
}
