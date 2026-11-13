// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

use algo_iterate_in_chunks::chunks;

#[test]
fn cases() {
    let cases: Vec<(&str, &[i32], usize, Vec<Vec<i32>>)> = vec![
        ("pairs", &[1, 2, 3, 4, 5], 2, vec![vec![1, 2], vec![3, 4], vec![5]]),
        ("triples", &[1, 2, 3, 4, 5, 6], 3, vec![vec![1, 2, 3], vec![4, 5, 6]]),
        ("empty", &[], 3, vec![]),
        ("size-one", &[7, 8, 9], 1, vec![vec![7], vec![8], vec![9]]),
        ("size-larger", &[1, 2], 5, vec![vec![1, 2]]),
        ("size-equal", &[1, 2, 3], 3, vec![vec![1, 2, 3]]),
        ("last-shorter", &[1, 2, 3, 4], 3, vec![vec![1, 2, 3], vec![4]]),
        ("negative", &[-1, -2, -3], 2, vec![vec![-1, -2], vec![-3]]),
        ("extremes", &[-2147483648, 2147483647, 0], 2, vec![vec![-2147483648, 2147483647], vec![0]]),
    ];
    for (name, items, size, want) in cases {
        assert_eq!(chunks(items, size), want, "{name}");
    }
}

#[test]
#[should_panic]
fn size_zero_panics() {
    chunks(&[1], 0);
}
