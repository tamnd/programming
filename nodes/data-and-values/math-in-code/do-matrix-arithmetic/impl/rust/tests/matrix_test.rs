// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

use algo_do_matrix_arithmetic::{matrix_multiply, transpose};

type Case = (&'static str, &'static [&'static [i64]], &'static [&'static [i64]], &'static [&'static [i64]]);

const CASES: &[Case] = &[
    ("two-by-two", &[&[1, 2], &[3, 4]], &[&[5, 6], &[7, 8]], &[&[19, 22], &[43, 50]]),
    ("two-by-three-times-three-by-two", &[&[1, 2, 3], &[4, 5, 6]], &[&[7, 8], &[9, 10], &[11, 12]], &[&[58, 64], &[139, 154]]),
    ("one-by-one", &[&[3]], &[&[4]], &[&[12]]),
    ("row-times-column", &[&[1, 2, 3]], &[&[4], &[5], &[6]], &[&[32]]),
    ("column-times-row", &[&[1], &[2], &[3]], &[&[4, 5, 6]], &[&[4, 5, 6], &[8, 10, 12], &[12, 15, 18]]),
    ("identity", &[&[1, 0], &[0, 1]], &[&[5, 6], &[7, 8]], &[&[5, 6], &[7, 8]]),
    ("other-order", &[&[5, 6], &[7, 8]], &[&[1, 2], &[3, 4]], &[&[23, 34], &[31, 46]]),
    ("zero-matrix", &[&[0, 0], &[0, 0]], &[&[1, 2], &[3, 4]], &[&[0, 0], &[0, 0]]),
    ("negative-numbers", &[&[-1, 2], &[3, -4]], &[&[2, 0], &[1, -1]], &[&[0, -2], &[2, 4]]),
    ("beyond-32-bit", &[&[100000, 0], &[0, 100000]], &[&[100000], &[3]], &[&[10000000000], &[300000]]),
];

#[test]
fn cases() {
    for &(name, a, b, want) in CASES {
        let got = matrix_multiply(a, b).expect(name);
        assert_eq!(got, want, "{name}");
    }
}

#[test]
fn transpose_rows_to_columns() {
    let m: &[&[i64]] = &[&[1, 2, 3], &[4, 5, 6]];
    assert_eq!(transpose(m), vec![vec![1, 4], vec![2, 5], vec![3, 6]]);
}

#[test]
fn sizes_must_match() {
    let a: &[&[i64]] = &[&[1, 2]];
    assert_eq!(matrix_multiply(a, a), None);
}
