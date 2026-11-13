// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

use algo_map_over_a_list::square_all;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], &[i32])] = &[
        ("small", &[1, 2, 3], &[1, 4, 9]),
        ("tens", &[5, 10], &[25, 100]),
        ("empty", &[], &[]),
        ("single", &[7], &[49]),
        ("negative", &[-3, -1], &[9, 1]),
        ("zero", &[0], &[0]),
        ("limit", &[46340, -46340], &[2147395600, 2147395600]),
        ("order", &[3, 1, 2], &[9, 1, 4]),
        ("duplicates", &[2, 2], &[4, 4]),
    ];
    for &(name, items, want) in cases {
        assert_eq!(square_all(items), want, "{name}");
    }
}
