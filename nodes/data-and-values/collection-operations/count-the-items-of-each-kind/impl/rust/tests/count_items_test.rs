// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

use algo_count_the_items_of_each_kind::count_items;

#[test]
fn cases() {
    let cases: Vec<(&str, &[i32], Vec<(i32, usize)>)> = vec![
        ("mixed", &[3, 1, 3, 2, 1, 3], vec![(1, 2), (2, 1), (3, 3)]),
        ("same", &[5, 5, 5], vec![(5, 3)]),
        ("empty", &[], vec![]),
        ("single", &[7], vec![(7, 1)]),
        ("negative", &[-1, 2, -1, 0], vec![(-1, 2), (0, 1), (2, 1)]),
        ("distinct", &[4, 2, 9], vec![(2, 1), (4, 1), (9, 1)]),
        ("order", &[9, 8, 9], vec![(8, 1), (9, 2)]),
        ("zero", &[0, 0], vec![(0, 2)]),
        ("extremes", &[2147483647, -2147483648, 2147483647], vec![(-2147483648, 1), (2147483647, 2)]),
    ];
    for (name, items, want) in cases {
        assert_eq!(count_items(items), want, "{name}");
    }
}
