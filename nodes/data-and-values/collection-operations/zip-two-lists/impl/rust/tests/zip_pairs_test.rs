// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

use algo_zip_two_lists::zip_pairs;

#[test]
fn cases() {
    let cases: Vec<(&str, &[i32], &[i32], Vec<(i32, i32)>)> = vec![
        ("same-length", &[1, 2, 3], &[4, 5, 6], vec![(1, 4), (2, 5), (3, 6)]),
        ("tens", &[10, 20], &[30, 40], vec![(10, 30), (20, 40)]),
        ("empty", &[], &[], vec![]),
        ("first-empty", &[], &[1, 2], vec![]),
        ("second-empty", &[1, 2], &[], vec![]),
        ("first-shorter", &[1], &[7, 8], vec![(1, 7)]),
        ("second-shorter", &[1, 2, 3], &[9], vec![(1, 9)]),
        ("negative", &[-1, -2], &[3, -4], vec![(-1, 3), (-2, -4)]),
        ("extremes", &[-2147483648, 2147483647], &[2147483647, -2147483648], vec![(-2147483648, 2147483647), (2147483647, -2147483648)]),
        ("duplicates", &[5, 5], &[5, 5], vec![(5, 5), (5, 5)]),
    ];
    for (name, a, b, want) in cases {
        assert_eq!(zip_pairs(a, b), want, "{name}");
    }
}
