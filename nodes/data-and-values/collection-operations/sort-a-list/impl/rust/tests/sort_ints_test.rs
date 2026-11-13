// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

use algo_sort_a_list::sort_ints;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], &[i32])] = &[
        ("small", &[3, 1, 2], &[1, 2, 3]),
        ("five", &[5, 4, 1, 3, 2], &[1, 2, 3, 4, 5]),
        ("empty", &[], &[]),
        ("single", &[1], &[1]),
        ("sorted", &[1, 2, 3], &[1, 2, 3]),
        ("reversed", &[9, 8, 7], &[7, 8, 9]),
        ("duplicates", &[2, 1, 2, 1], &[1, 1, 2, 2]),
        ("negative", &[-1, -10, 5], &[-10, -1, 5]),
        ("digits", &[10, 9, 100, 1], &[1, 9, 10, 100]),
        ("extremes", &[2147483647, -2147483648, 0], &[-2147483648, 0, 2147483647]),
    ];
    for &(name, items, want) in cases {
        assert_eq!(sort_ints(items), want, "{name}");
    }
}
