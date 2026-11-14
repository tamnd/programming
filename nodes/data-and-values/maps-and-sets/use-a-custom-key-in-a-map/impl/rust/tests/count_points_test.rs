// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

use algo_use_a_custom_key_in_a_map::count_points;

#[test]
fn cases() {
    let cases: Vec<(&str, Vec<(i32, i32)>, Vec<(i32, i32, usize)>)> = vec![
        ("clicks", vec![(1, 2), (3, 4), (1, 2)], vec![(1, 2, 2), (3, 4, 1)]),
        ("swap", vec![(2, 1), (1, 2)], vec![(1, 2, 1), (2, 1, 1)]),
        ("empty", vec![], vec![]),
        ("one", vec![(0, 0)], vec![(0, 0, 1)]),
        ("negative", vec![(-1, 0), (0, -1), (-1, 0)], vec![(-1, 0, 2), (0, -1, 1)]),
        ("joined-digits", vec![(1, 23), (12, 3)], vec![(1, 23, 1), (12, 3, 1)]),
        ("all-same", vec![(5, 5), (5, 5), (5, 5)], vec![(5, 5, 3)]),
    ];
    for (name, points, want) in cases {
        assert_eq!(count_points(&points), want, "{name}");
    }
}
