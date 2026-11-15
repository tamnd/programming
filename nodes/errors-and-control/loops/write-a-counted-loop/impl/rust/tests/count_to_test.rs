// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

use algo_write_a_counted_loop::count_to;

#[test]
fn cases() {
    let cases: &[(&str, i32, &[i32])] = &[
        ("ten", 10, &[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]),
        ("three", 3, &[1, 2, 3]),
        ("zero", 0, &[]),
        ("one", 1, &[1]),
        ("two", 2, &[1, 2]),
    ];
    for &(name, n, want) in cases {
        assert_eq!(count_to(n), want, "{name}");
    }
}
