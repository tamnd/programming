// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

use algo_wait_for_threads_to_finish::parallel_sums;

#[test]
fn cases() {
    let g = 1_000_000_000;
    let cases: Vec<(&str, Vec<Vec<i64>>, Vec<i64>)> = vec![
        ("two", vec![vec![1, 2, 3], vec![4, 5]], vec![6, 9]),
        ("three", vec![vec![10], vec![20, 30], vec![-5, 5]], vec![10, 50, 0]),
        ("empty", vec![], vec![]),
        ("empty-chunk", vec![vec![], vec![7]], vec![0, 7]),
        ("single", vec![vec![42]], vec![42]),
        ("negative", vec![vec![-1, -2], vec![-3]], vec![-3, -3]),
        ("limit", vec![vec![g; 3], vec![-g; 3]], vec![3 * g, -3 * g]),
        ("many", (0..32).map(|i| vec![i; 3]).collect(), (0..32).map(|i| 3 * i).collect()),
    ];
    for (name, chunks, want) in cases {
        assert_eq!(parallel_sums(&chunks), want, "case {name}");
    }
}
