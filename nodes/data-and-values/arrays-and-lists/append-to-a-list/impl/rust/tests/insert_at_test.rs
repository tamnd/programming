// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

use algo_append_to_a_list::insert_at;

#[test]
fn cases() {
    let cases: &[(&str, &[i32], usize, i32, &[i32])] = &[
        ("at-end", &[1, 2], 2, 3, &[1, 2, 3]),
        ("at-front", &[2, 3], 0, 1, &[1, 2, 3]),
        ("in-middle", &[1, 3], 1, 2, &[1, 2, 3]),
        ("empty", &[], 0, 7, &[7]),
        ("single-front", &[5], 0, 4, &[4, 5]),
        ("single-end", &[5], 1, 6, &[5, 6]),
        ("end-of-many", &[1, 2, 3, 4], 4, 5, &[1, 2, 3, 4, 5]),
        ("front-of-many", &[1, 2, 3, 4], 0, 0, &[0, 1, 2, 3, 4]),
        ("duplicates", &[1, 1], 1, 1, &[1, 1, 1]),
        ("limit", &[2147483647], 1, i32::MIN, &[2147483647, i32::MIN]),
    ];
    for &(name, items, index, value, want) in cases {
        assert_eq!(insert_at(items, index, value), want, "{name}");
    }
}

#[test]
#[should_panic]
fn index_past_the_end() {
    insert_at(&[1, 2], 3, 9);
}
