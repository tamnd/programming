// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

use algo_do_complex_number_arithmetic::{complex_arithmetic, Complex};

const CASES: &[(&str, &[f64], &[f64], &[f64])] = &[
    ("textbook", &[3.0, 4.0], &[1.0, -2.0], &[4.0, 2.0, 11.0, -2.0, -1.0, 2.0, 5.0, 0.9272952180016122]),
    ("rosetta", &[1.0, 1.0], &[3.14159, 1.25], &[4.14159, 2.25, 1.8915899999999999, 4.39159, 0.38414593243590134, 0.16546321590504276, 1.4142135623730951, 0.7853981633974483]),
    ("real-only", &[6.0, 0.0], &[3.0, 0.0], &[9.0, 0.0, 18.0, 0.0, 2.0, 0.0, 6.0, 0.0]),
    ("imaginary-unit", &[0.0, 1.0], &[0.0, 1.0], &[0.0, 2.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.5707963267948966]),
    ("negative-real", &[-2.0, 0.0], &[1.0, 0.0], &[-1.0, 0.0, -2.0, 0.0, -2.0, 0.0, 2.0, 3.141592653589793]),
    ("zero", &[0.0, 0.0], &[2.0, -3.0], &[2.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]),
    ("negative-phase", &[1.0, -1.0], &[-1.0, -1.0], &[0.0, -2.0, -2.0, 0.0, 0.0, 1.0, 1.4142135623730951, -0.7853981633974483]),
    ("imaginary-divisor", &[2.0, 4.0], &[0.0, 2.0], &[2.0, 6.0, -8.0, 4.0, 2.0, -1.0, 4.47213595499958, 1.1071487177940904]),
    ("large-divisor", &[1e+150, 1e+150], &[1e+155, 1e+155], &[1.00001e+155, 1.00001e+155, 0.0, 2e+305, 9.999999999999999e-06, 0.0, 1.414213562373095e+150, 0.7853981633974483]),
];

fn close(got: f64, want: f64) -> bool {
    (got - want).abs() <= f64::max(1e-9, 1e-12 * want.abs())
}

#[test]
fn cases() {
    for &(name, a, b, want) in CASES {
        let got = complex_arithmetic(a, b);
        for (i, (g, w)) in got.iter().zip(want).enumerate() {
            assert!(close(*g, *w), "{name}: item {i} = {g}");
        }
    }
}

#[test]
fn i_squared() {
    let i = Complex { re: 0.0, im: 1.0 };
    assert_eq!(i * i, Complex { re: -1.0, im: 0.0 });
}
