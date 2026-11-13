// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

use algo_rotate_a_list::rotate_left;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], usize, &[i32])] = &[
        ("by-two", &[1, 2, 3, 4, 5], 2, &[3, 4, 5, 1, 2]),
        ("by-one", &[1, 2, 3], 1, &[2, 3, 1]),
        ("zero", &[1, 2, 3], 0, &[1, 2, 3]),
        ("full-turn", &[1, 2, 3], 3, &[1, 2, 3]),
        ("more-than-length", &[1, 2, 3], 4, &[2, 3, 1]),
        ("much-larger", &[1, 2, 3], 2000000000, &[3, 1, 2]),
        ("empty", &[], 5, &[]),
        ("empty-zero", &[], 0, &[]),
        ("single", &[7], 3, &[7]),
        ("duplicates", &[1, 1, 2], 1, &[1, 2, 1]),
        ("negative-items", &[-1, -2, -3], 1, &[-2, -3, -1]),
    ];
    for &(name, items, k, want) in cases {
        assert_eq!(rotate_left(items, k), want, "{name}");
    }
}
