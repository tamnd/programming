// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

use algo_create_an_array::filled;

#[test]
fn cases() {
    let cases: &[(&str, usize, i32, &[i32])] = &[
        ("three-sevens", 3, 7, &[7, 7, 7]),
        ("five-zeros", 5, 0, &[0, 0, 0, 0, 0]),
        ("empty", 0, 7, &[]),
        ("single", 1, 42, &[42]),
        ("zero", 2, 0, &[0, 0]),
        ("negative", 2, -1, &[-1, -1]),
        ("max-int", 2, 2147483647, &[2147483647, 2147483647]),
        ("min-int", 2, i32::MIN, &[i32::MIN, i32::MIN]),
        ("ten", 10, 1, &[1, 1, 1, 1, 1, 1, 1, 1, 1, 1]),
    ];
    for &(name, n, value, want) in cases {
        assert_eq!(filled(n, value), want, "{name}");
    }
}
