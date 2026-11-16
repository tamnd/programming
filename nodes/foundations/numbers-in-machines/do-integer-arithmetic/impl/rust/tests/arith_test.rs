// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

use algo_do_integer_arithmetic::arith;

#[test]
fn cases() {
    let cases: Vec<(&str, i64, i64, u32, [i64; 6])> = vec![
        ("small", 7, 2, 3, [9, 5, 14, 3, 1, 343]),
        ("exact-division", 20, 5, 2, [25, 15, 100, 4, 0, 400]),
        ("negative-dividend", -7, 2, 3, [-5, -9, -14, -3, -1, -343]),
        ("negative-divisor", 7, -2, 2, [5, 9, -14, -3, 1, 49]),
        ("both-negative", -7, -2, 1, [-9, -5, 14, 3, -1, -7]),
        ("zero-dividend", 0, 5, 0, [5, -5, 0, 0, 0, 1]),
        ("zero-power", 5, 3, 0, [8, 2, 15, 1, 2, 1]),
        ("divisor-larger", 3, 7, 1, [10, -4, 21, 0, 3, 3]),
        ("largest", 100000, 100000, 3, [200000, 0, 10000000000, 1, 0, 1000000000000000]),
        ("smallest", -100000, 1, 3, [-99999, -100001, -100000, -100000, 0, -1000000000000000]),
    ];
    for (name, a, b, n, want) in cases {
        assert_eq!(arith(a, b, n), want, "case {name}");
    }
}

#[test]
fn euclid_division_differs() {
    assert_eq!((-7i64).div_euclid(2), -4);
    assert_eq!((-7i64).rem_euclid(2), 1);
}
